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
 * @brief Interface for MovementSystem
*/

#ifndef COMPUBRITE_SFML_MOVEMENTSYSTEM_H
#define COMPUBRITE_SFML_MOVEMENTSYSTEM_H

#include <CompuBrite/SFML/ISystem.h>

namespace CompuBrite::SFML {

/// Specialization of ISystem for handling movement of IEntity objects.
class MovementSystem : public CompuBrite::SFML::ISystem
{
public:
    MovementSystem() = default;
    virtual ~MovementSystem() = default;

    /// Update the position of all managed IEntity objects by applying their
    /// acceleration, rotation, and other properties.  Perform the required
    /// transforms for each IEntity object.
    /// @param dt The elapsed time since the last call to update().
    void update(Context &target, sf::Time dt) override;

private:

    /// Add properties to the given IEntity object if they are not already
    /// present.
    /// The following properties will be added: "velocity", "rotation",
    /// "acceleration", "rot_accel", (rotational acceleration).
    /// The update() method will use these properties to provide the proper
    /// transformations to move, and rotate the IEntity object.
    /// @param entity The IEntity object to add the required properties to.
    void addProperties(IEntity &entity) override;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_MOVEMENTSYSTEM_H
