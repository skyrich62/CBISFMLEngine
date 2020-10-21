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
 * @brief Implementation for IShapeEntity
*/

#include "CompuBrite/SFML/IShapeEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite::SFML {

IShapeEntity::IShapeEntity(sf::Shape &shape) :
    shape_(&shape)
{
    //ctor
}

void
IShapeEntity::setTexture(const sf::Texture *texture, bool resetRect)
{
    shape_->setTexture(texture, resetRect);
}

void
IShapeEntity::setTextureRect(const sf::IntRect &rectangle)
{
    shape_->setTextureRect(rectangle);
}

void
IShapeEntity::setFillColor(const sf::Color &color)
{
    shape_ ->setFillColor(color);
}

void
IShapeEntity::setOutlineColor(const sf::Color &color)
{
    shape_ ->setOutlineColor(color);
}

void
IShapeEntity::setOutlineThickness(float thickness)
{
    shape_->setOutlineThickness(thickness);
}

const sf::Texture *
IShapeEntity::getTexture() const
{
    return shape_->getTexture();
}

const sf::IntRect &
IShapeEntity::getTextureRect() const
{
    return shape_->getTextureRect();
}

const sf::Color &
IShapeEntity::getFillColor() const
{
    return shape_->getFillColor();
}

const sf::Color &
IShapeEntity::getOutlineColor() const
{
    return shape_->getOutlineColor();
}

float
IShapeEntity::getOutlineThickness() const
{
    return shape_->getOutlineThickness();
}

std::size_t
IShapeEntity::getPointCount() const
{
    return shape_->getPointCount();
}

sf::Vector2f
IShapeEntity::getPoint(std::size_t index) const
{
    return shape_->getPoint(index);
}

void
IShapeEntity::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*shape_, states);
}

sf::FloatRect
IShapeEntity::getLocalBounds() const
{
    return shape_->getLocalBounds();
}

} // namespace CompuBrite::SFML
