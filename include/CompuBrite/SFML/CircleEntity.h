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
 * @brief Interface for CircleEntity
*/

#ifndef COMPUBRITE_SMFL_CIRCLEENTITY_H
#define COMPUBRITE_SMFL_CIRCLEENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/CircleShape.hpp>

namespace CompuBrite::SFML {

/// A CircleEntity encapsulates sf::CircleShape.
/// @see IEntity
class CircleEntity : public CompuBrite::SFML::IShapeEntity
{
public:
    /// Construct a CircleEntity with the given radius and pointCount.
    /// @param radius The radius of the circle.
    /// @param pointCount The number of points around the circumference of
    /// the circle.
    explicit CircleEntity(float radius = 0.f, std::size_t pointCount = 30);

    /// Set a new radius for the circle.
    /// @param radius The new radius.
    void setRadius(float radius);

    /// Retreive the current radius.
    /// @return The current radius.
    float getRadius() const;

    /// Set a new point count for the circle.
    void setPointCount(std::size_t count);

protected:

    sf::CircleShape circle_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SMFL_CIRCLEENTITY_H
