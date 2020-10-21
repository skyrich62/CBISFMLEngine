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
 * @brief Interface for ConvexEntity
*/

#ifndef COMPUBRITE_SFML_CONVEXENTITY_H
#define COMPUBRITE_SFML_CONVEXENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/ConvexShape.hpp>

namespace CompuBrite::SFML {

/// A ConvexEntity encapsulates sf::ConvexShape
/// @see IEntity.
class ConvexEntity : public IShapeEntity
{
public:
    /// Construct the ConvexShape with the given pointCount.
    /// @param pointCount The number of vertex points.
    explicit ConvexEntity(std::size_t pointCount = 0);

    /// Set a new point count.
    void setPointCount(std::size_t count);

    /// Set the given vertex to the given x, y coordinates.
    /// @param index The vertex to set.
    /// @param point The x, y coordinates to set it to.
    void setPoint(std::size_t index, const sf::Vector2f &point);

protected:

private:
    sf::ConvexShape shape_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_CONVEXENTITY_H
