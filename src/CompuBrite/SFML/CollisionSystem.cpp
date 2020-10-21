/**
 * The MIT License (MIT)
 *
 * @copyright
 * Copyright (c) 2020 Rich Newman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file
 * @brief Implementation for CollisionSystem
*/

#include "CompuBrite/SFML/CollisionSystem.h"

#include <array>
#include <ostream>

std::ostream&
operator<<(std::ostream &os, const sf::Vector2f &v)
{
    os << '[' << v.x << ", " << v.y << ']';
    return os;
}

namespace CompuBrite::SFML {

static bool
satRectangleAndPoints(const sf::Vector2f &rectangleSize,
                      const std::array<sf::Vector2f, 4> &points)
{
    auto allPointsLeftOfRectangle = true;
    auto allPointsRightOfRectangle = true;
    auto allPointsAboveRectangle = true;
    auto allPointsBelowRectangle = true;

    for (const auto &point : points) {
        if (point.x >= 0.f) {
            allPointsLeftOfRectangle = false;
        }
        if (point.x <= rectangleSize.x) {
            allPointsRightOfRectangle = false;
        }
        if (point.y >= 0.f) {
            allPointsAboveRectangle = false;
        }
        if (point.y <= rectangleSize.y) {
            allPointsBelowRectangle = false;
        }
    }
    return !(allPointsAboveRectangle || allPointsBelowRectangle ||
             allPointsLeftOfRectangle || allPointsRightOfRectangle);
}

CollisionSystem::CollisionSystem(Level level) :
    level_(level)
{
}

void
CollisionSystem::update(sf::Time dt)
{
    checkCollisions();
}

CollisionSystem::Handler
CollisionSystem::find(const TypeIDs &p) const
{
    auto found = handlers_.find(p);
    return (found == handlers_.end()) ? Handler() : found->second;
}

void
CollisionSystem::handleCollision(IEntity &lhs, IEntity &rhs, const sf::FloatRect &rect) const
{
    auto t1 = &typeid(lhs);
    auto t2 = &typeid(rhs);
    auto p = std::minmax(t1, t2);
    auto handler = find(p);
    if (!handler.second) {
        CheckPoint::hit(CBI_HERE, "no handler");
        return;
    }
    if ((t2 < t1) == handler.first) {
        handler.second(lhs, rhs, rect);
    } else {
        handler.second(rhs, lhs, rect);
    }
}

bool
CollisionSystem::didCollide(const IEntity &lhs, const IEntity &rhs, sf::FloatRect &rect) const
{
    if (!CheckPoint::expect(CBI_HERE, &lhs != &rhs, "lhs == rhs.  Shouldn't have happened!")) {
        // objects don't collide with themselves.
        return false;
    }

    // Now check level 0, (AABB)
    const auto level0 = lhs.getGlobalBounds().intersects(rhs.getGlobalBounds(), rect);
    if (!level0 || level_ == AABB) {
        // If they don't intersect, or we're only checking AABB return now.
        return level0;
    }

    // The AABBs intersect, now check if the BBs intersect;
    const auto leftTransform = lhs.getGlobalTransform();
    const auto rightTransform = rhs.getGlobalTransform();
    const auto leftInverseTransform = leftTransform.getInverse();
    const auto rightInverseTransform = rightTransform.getInverse();
    const auto leftBounds = lhs.getLocalBounds();
    const auto rightBounds = rhs.getLocalBounds();

    const sf::Vector2f leftSize {leftBounds.width, leftBounds.height };
    const sf::Vector2f rightSize {rightBounds.width, rightBounds.height };

    auto point = sf::Vector2f(0.f, 0.f);
    point = leftTransform.transformPoint( point );
    const auto leftTopLeft = rightInverseTransform.transformPoint(point);

    point = sf::Vector2f(leftSize.x, 0.f);
    point = leftTransform.transformPoint( point );
    const auto leftTopRight = rightInverseTransform.transformPoint(point);

    point = leftTransform.transformPoint(leftSize);
    const auto leftBottomRight = rightInverseTransform.transformPoint(point);

    point = sf::Vector2f( 0.f, leftSize.y );
    point = leftTransform.transformPoint(point);
    const auto leftBottomLeft = rightInverseTransform.transformPoint(point);


    point = sf::Vector2f(0.f, 0.f );
    point = rightTransform.transformPoint(point);
    const auto rightTopLeft = leftInverseTransform.transformPoint(point);

    point = sf::Vector2f(rightSize.x, 0.f );
    point = rightTransform.transformPoint(point);
    const auto rightTopRight = leftInverseTransform.transformPoint(point);

    point = rightTransform.transformPoint(rightSize);
    const auto rightBottomRight = leftInverseTransform.transformPoint(point);

    point = sf::Vector2f( 0.f, rightSize.y );
    point = rightTransform.transformPoint(point);
    const auto rightBottomLeft = leftInverseTransform.transformPoint(point);

    const bool level1 =
        leftBounds.contains(rightTopLeft) ||
        leftBounds.contains(rightTopRight) ||
        leftBounds.contains(rightBottomLeft) ||
        leftBounds.contains(rightBottomRight) ||
        rightBounds.contains(leftTopLeft) ||
        rightBounds.contains(leftTopRight) ||
        rightBounds.contains(leftBottomLeft) ||
        rightBounds.contains(leftBottomRight);


    if (!level1 || level_ == BB) {
        // The transformed boundary rectangles don't intersect or we're only
        // checking for that.
        return level1;
    }

    // Now apply the Separating Axis Theorem, (SAT).
    std::array<sf::Vector2f, 4> leftPoints
    {
        leftBottomLeft,
        leftBottomRight,
        leftTopRight,
        leftTopLeft
    };

    std::array<sf::Vector2f, 4> rightPoints
    {
        rightBottomLeft,
        rightBottomRight,
        rightTopRight,
        rightTopLeft
    };

    return satRectangleAndPoints(rightSize, leftPoints) &&
           satRectangleAndPoints(leftSize, rightPoints);
}

void
CollisionSystem::checkCollisions() const
{
    if (entities_.size() < 2) {
        // Need at least 2 entities to have a collision.
        return;
    }
    for (auto first = entities_.begin(); first != entities_.end(); ++first) {
        auto next = first;
        for (++next; next != entities_.end(); ++next) {
            sf::FloatRect rect;
            if (didCollide(**first, **next, rect)) {
                //CheckPoint::hit(CBI_HERE, "Collision");
                handleCollision(**first, **next, rect);
            }
        }
    }

}

} // namespace CompuBrite::SFML
