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
 * @brief Interface for DrawingSystem
*/

#ifndef COMPUBRITE_SFML_DRAWINGSYSTEM_H
#define COMPUBRITE_SFML_DRAWINGSYSTEM_H

#include <CompuBrite/SFML/ISystem.h>

namespace CompuBrite::SFML {

/// A system to draw IEntity objects.  All IEntity objects added to this
/// system will be drawn on the given target when the draw() method is called.
class DrawingSystem : public CompuBrite::SFML::ISystem
{
public:
    /// Construct the drawing system.
    /// @param boundingBoxes If true, then draw Axis-Aligned Bounding Boxes
    /// around all entities.  This is useful for debugging, but very slow.
    explicit DrawingSystem(bool boundingBoxes = false);
    virtual ~DrawingSystem() = default;

protected:
    /// Draw all of the IEntity objects assigned to this DrawingSystem.
    /// This will call each IEntity's draw() method, passing along the target
    /// and states.
    /// @param target Draw them to this target
    /// @param states Use these base states.
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    bool boundingBoxes_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_DRAWINGSYSTEM_H
