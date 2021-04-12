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
 * @brief Implementation for IEntity
*/

#include "CompuBrite/SFML/IEntity.h"
#include "CompuBrite/SFML/ISystem.h"

#include <map>

namespace CompuBrite::SFML {

sf::FloatRect
IEntity::getLocalBounds() const
{
    return sf::FloatRect();
}

void
IEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    if (true) {
        auto l = lock();
        drawThis(target, states);
    }
    drawChildren(target, states);
}

void
IEntity::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::multimap<int, IEntity*> drawings;
    if (true) {
        auto l = lock();
        for (auto child : children_) {
            if (child) {
                drawings.insert(std::pair<int, IEntity*>(child->zOrder(), child));
            }
        }
    }
    for (auto child : drawings) {
        child.second->draw(target, states);
    }
}

bool
IEntity::addChild(IEntity &child)
{
    auto l = lock();
    auto found = std::find(children_.begin(), children_.end(), &child);
    if (found != children_.end()) {
        return false;
    }
    children_.push_back(&child);
    auto l2 = child.lock();
    child.parent_ = this;
    return true;
}

bool
IEntity::addChild(IEntity &child, int zOrder)
{
    auto ret = addChild(child);
    if (ret) {
        child.zOrder(zOrder);
    }
    return ret;
}

void
IEntity::drawThis(sf::RenderTarget &, sf::RenderStates) const
{
}

void
IEntity::update(sf::Time dt)
{
    if (true) {
        auto l = lock();
        updateThis(dt);
    }
    updateChildren(dt);
}

void
IEntity::updateChildren(sf::Time dt)
{
    for (auto child : children_) {
        child->update(dt);
    }
}

void
IEntity::updateThis(sf::Time dt)
{
}

void
IEntity::addSystem(ISystem &system, bool callback)
{
    auto l = lock();
    auto found = find(systems_.begin(), systems_.end(), &system);
    if (found == systems_.end()) {
        systems_.push_back(&system);
        l.unlock();
        if (callback) {
            system.addEntity(*this, false);
        }
    }
}

void
IEntity::dropSystem(ISystem &system, bool callback)
{
    auto l = lock();
    auto found = find(systems_.begin(), systems_.end(), &system);
    if (found != systems_.end()) {
        systems_.erase(found);
        l.unlock();
        if (callback) {
            system.dropEntity(*this, false);
        }
    }
}

sf::Transform
IEntity::getGlobalTransform() const
{
    auto transform = sf::Transform::Identity;
    for (auto entity = this; entity;) {
        auto l = entity->lock();
        transform = entity->getTransform() * transform;
        entity = entity->parent_;
    }
    return transform;
}

sf::Vector2f
IEntity::getGlobalPosition() const
{
    auto transform = getGlobalTransform();
    auto l = lock();
    return transform * this->getOrigin();
}

sf::FloatRect
IEntity::getGlobalBounds() const
{
    auto transform = getGlobalTransform();
    auto l = lock();
    return transform.transformRect(getLocalBounds());
}


} // namespace CompuBrite::SFML
