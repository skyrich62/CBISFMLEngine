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
 * @brief Implementation for SpriteEntity
*/

#include "CompuBrite/SFML/SpriteEntity.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite::SFML {

SpriteEntity::SpriteEntity(int zOrder, const sf::Texture &texture) :
    IEntity(zOrder),
    sprite_(texture)
{
}

SpriteEntity::SpriteEntity(int zOrder, const sf::Texture &texture, const sf::IntRect &rect) :
    IEntity(zOrder),
    sprite_(texture, rect)
{
}

SpriteEntity::SpriteEntity(const sf::Texture &texture) :
    SpriteEntity(0, texture)
{
}

SpriteEntity::SpriteEntity(const sf::Texture &texture, const sf::IntRect &rect) :
    SpriteEntity(0, texture, rect)
{
}

void
SpriteEntity::setTexture(const sf::Texture &texture, bool resetRect)
{
    sprite_.setTexture(texture, resetRect);
}

void
SpriteEntity::setTextureRect(const sf::IntRect &rectangle)
{
    sprite_.setTextureRect(rectangle);
}

const sf::Texture *
SpriteEntity::getTexture() const
{
    return sprite_.getTexture();
}

const sf::IntRect &
SpriteEntity::getTextureRect() const
{
    return sprite_.getTextureRect();
}

const sf::Color &
SpriteEntity::getColor() const
{
    sprite_.getColor();
}

sf::FloatRect
SpriteEntity::getLocalBounds() const
{
    return sprite_.getLocalBounds();
}

void
SpriteEntity::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}


} // namespace CompuBrite::SFML
