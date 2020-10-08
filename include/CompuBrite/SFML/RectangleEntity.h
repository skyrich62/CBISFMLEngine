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
 * @brief Interface for RectangleEntity
*/

#ifndef COMPUBRITE_SFML_RECTANGLEENTITY_H
#define COMPUBRITE_SFML_RECTANGLEENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/RectangleShape.hpp>

namespace CompuBrite
{
namespace SFML
{

/// Encapsulate a sf::RectangleShape and bind it to an IEntity object for
/// drawing and transforming it.
/// @see IEntity
/// @see IShapeEntity
class RectangleEntity : public CompuBrite::SFML::IShapeEntity
{
public:
    /// Construct the sf::RectangleShape object.
    /// @param size The size of the RectangleShape object.
    RectangleEntity(const sf::Vector2f &size = sf::Vector2f(0.0f, 0.0f));

    /// Set a new size for the sf::RectangleShape.  This just delegates to
    /// sf::RectangleShape::setSize()
    /// @param size The new size for the rectangle.
    void setSize(const sf::Vector2f &size);

    /// Get the size of the sf::RectangleShape object.  Deletegates to
    /// sf::RectangleShape::getSize()
    const sf::Vector2f &getSize() const;

protected:

private:
    sf::RectangleShape rectangle_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_RECTANGLEENTITY_H
