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
 * @brief Interface for ISystem.
*/
#ifndef COMPUBRITE_SFML_ISYSTEM_H
#define COMPUBRITE_SFML_ISYSTEM_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include <CompuBrite/SFML/IEntity.h>

#include <vector>

namespace CompuBrite
{
namespace SFML
{

/// ISystem is the common interface for managing various IEntity objects.
/// An IEntity can be registered with multiple ISystem objects, and each
/// ISystem can manage multiple IEntity objects.
class ISystem : public sf::Drawable
{
public:
    ISystem();
    virtual ~ISystem();

    /// Add an IEntity object to this ISystem.
    /// @param entity The IEntity to add.
    /// @param callback Used for internal processing.
    void addEntity(IEntity& entity, bool callback = true);

    /// Remove an IEntity from this ISystem.
    /// @param entity The IEntity to remove.
    /// @param callback Used for internal processing.
    void dropEntity(IEntity& entity, bool callback = true);

    /// Draw this ISystem.  By default does nothing, subclasses must override
    /// to provide required processing.
    /// @see DrawingSystem
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// Update this ISystem.  By default does nothing, subclasses must override
    /// to provide required processing.
    /// @see MovementSystem
    /// @see CollisionSystem
    virtual void update(sf::Time dt);

protected:
    std::vector<IEntity*> entities_;

private:
    /// Add any required properties to the given IEntity object. This is called
    /// from addEntity.  By default does nothing, subclasses must override to
    /// provide required processing.
    /// @see MovementSystem
    /// @see PropertyManager
    /// @see TProperty
    /// @param entity The entity to process and add properties.
    virtual void addProperties(IEntity &entity);
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ISYSTEM_H
