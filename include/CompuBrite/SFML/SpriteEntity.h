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
 * @brief Interface for SpriteEntity
*/

#ifndef COMPUBRITE_SFML_SPRITEENTITY_H
#define COMPUBRITE_SFML_SPRITEENTITY_H

#include <CompuBrite/SFML/IEntity.h>
#include <SFML/Graphics/Sprite.hpp>

namespace CompuBrite
{
namespace SFML
{

/// Encapsulates sf::Sprite into the IEntity framework.
class SpriteEntity : public CompuBrite::SFML::IEntity
{
public:
    /// Construct an empty sf::Sprite.
    SpriteEntity() = default;

    /// Construct a sf::Sprite object with the given sf::Texture.
    /// @param texture The requested sf::Texture to associate to this sprite.
    explicit SpriteEntity(const sf::Texture &texture);

    /// @override
    /// Construct a sf::Sprite object with the given part of the sf::Texture.
    /// @param texture The requested sf::Texture.
    /// @param rectangle The rectangle describing the part of the texture to use.
    SpriteEntity(const sf::Texture &texture, const sf::IntRect &rectangle);

    /// @override
    /// Also provide zOrder
    SpriteEntity(int zOrder, const sf::Texture &texture);

    /// @override
    /// Also provide zOrder
    SpriteEntity(int zOrder, const sf::Texture &texture, const sf::IntRect &rectangle);

    virtual ~SpriteEntity() = default;

    /// @return the Axis-Aligned Boundary Box for this Sprite.
    sf::FloatRect getLocalBounds() const override;

    /// Set the texture of the sf::Sprite.
    /// This merely delegates to sf::Sprite::setTexture().
    void setTexture(const sf::Texture &texture, bool resetRect = false);

    /// Set the texture rectangle. Delegates to sf::Sprite::setTextureRect().
    void setTextureRect(const sf::IntRect &rectangle);

    /// Set the color of the sprite. Delegates to sf::Sprite::setColor().
    void setColor(const sf::Color &color);

    /// Get the texture associated with this sprite.
    /// Delegates to sf::Sprite::getTexture().
    const sf::Texture *getTexture() const;

    /// Get the texture rectangle.  Delegates to sf::Sprite::getTextureRect().
    const sf::IntRect &getTextureRect() const;

    /// Get the sprite's color.  Delegates to sf::Sprite::getColor().
    const sf::Color &getColor() const;

private:

    /// Draw this sprite.  Delegates to sf::Sprite::draw().
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Sprite sprite_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_SPRITEENTITY_H
