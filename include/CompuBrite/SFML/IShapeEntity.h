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
 * @brief Interface for IShapeEntity
*/

#ifndef COMPUBRITE_SFML_ISHAPEENTITY_H
#define COMPUBRITE_SFML_ISHAPEENTITY_H

#include <CompuBrite/SFML/IEntity.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace CompuBrite
{
namespace SFML
{

/// Forms the base class for CircleEntity, ConvexEntity, etc.
/// This class provides all of the common interface methods for sf::Shape, and
/// binds them with the IEntity interface.  This class delegates to sf::Shape
/// to provide those interface requirements.
/// @see sf::Shape.
class IShapeEntity : public CompuBrite::SFML::IEntity
{
public:
    virtual ~IShapeEntity() = default;

    /// Set the sf::Texture for this shape.
    /// @param texture a pointer to the desired texture, can be a nullptr
    /// to remove any texture from this IShapeEntity.
    /// @param resetRect Determines whether or not to reset the texture
    /// rectangle.
    /// @see sf::Shape::setTexture
    void setTexture(const sf::Texture *texture, bool resetRect = false);

    /// Set the texture rectangle for this IShapeEntity.
    /// @param rectangle The rectagle, of the texture to use to draw this
    /// IShapeEntity object.
    void setTextureRect(const sf::IntRect &rectangle);

    /// Set the fill color of this object.
    /// @param color The color to use.
    void setFillColor(const sf::Color &color);

    /// Set the outline color.
    /// @param color The color to use.
    void setOutlineColor(const sf::Color &color);

    /// Set the outline thickness.
    void setOutlineThickness(float thickness);

    /// @return The sf::Texture associated with this object if there is one,
    /// else nullptr.
    const sf::Texture *getTexture() const;

    /// @return The texture rectangle or an empty rectangle.
    const sf::IntRect &getTextureRect() const;

    /// @return The fill color.
    const sf::Color &getFillColor() const;

    /// @return the outline color
    const sf::Color &getOutlineColor() const;

    /// @return the outline thickness.
    float getOutlineThickness() const;

    /// @return The point count for this object.
    std::size_t getPointCount() const;

    /// @return the point coordinates at the given index.
    sf::Vector2f getPoint(std::size_t index) const;

    /// @return The local Axis-Aligned Boundary Box for this object.
    sf::FloatRect getLocalBounds() const override;

protected:
    /// IShapeEntity can only be constructed through one of it's derived
    /// objects.
    /// @param shape, The sf::Shape that is being managed through this
    /// interface.
    explicit IShapeEntity(sf::Shape &shape);

private:
    /// Draw the sf::Shape which is bound to this IShapeEntity object.
    /// This overrides IEntiry::draw() to provide the means to draw sf::Shape
    /// derived objects.
    /// @param target Where to draw
    /// @param states The sf::RenderStates to use while drawing.
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Shape *shape_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ISHAPEENTITY_H
