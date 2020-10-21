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
 * @brief Interface for IEntity
*/
#ifndef COMPUBRITE_SFML_ENTITY_H
#define COMPUBRITE_SFML_ENTITY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <CompuBrite/SFML/PropertyManager.h>


#include <vector>
#include <functional>

namespace CompuBrite::SFML {

class ISystem;

/// This class forms the base of the entire framework.  An IEntity is a
/// drawable, and transformable object, which may have child IEntity objects
/// which will be drawn and/or transformed relative to it.
class IEntity : public sf::Drawable, public sf::Transformable
{
public:

    IEntity() = default;

    /// Construct this IEntity with the given zOrder
    /// @param zOrder zOrder layering lower numbers will be drawn first, other
    /// than that, the zOrder is arbitrary.
    explicit IEntity(int zOrder) : zOrder_(zOrder) { }
    virtual ~IEntity();

    /// @return retrieve the local bounding box for this IEntity.  By default,
    /// IEntity will return an empty rectangle.  Derived classes *must*
    /// override this function to provide the local bounds for their custom
    /// entity, or else collision detection, (and some other things), will
    /// not work correctly.
    virtual sf::FloatRect getLocalBounds() const;

    /// Add a child IEntity object at the given zOrder.  This object will
    /// be drawn and or transformed relative to this, (the parent), object.
    /// Thus if the parent object is moved, the child objects will be moved
    /// as well.  Drawing the parent will cause the children to draw as well.
    /// @param child The IEnity object to add as a child.
    /// @param zOrder the drawing order to use for this child.  Lower numbers
    /// will be drawn first.  Thus, if this child's zOrder is lower than the
    /// parent, it will be drawn before the parent.
    /// @return true if the child object was successfully installed.
    bool addChild(IEntity &child, int zOrder);

    /// @overload.
    /// Add a child without specifying a new zOrder for it.
    /// @param child The child object to add.
    bool addChild(IEntity &child);

    /// Perform any custom updates for this IEntity object.  By default,
    /// This is an empty function.  Derived classes should override this
    /// function if custom object updating is desired.  Normally, updating
    /// is performed though the ISystem mechanism.
    /// @param dt the elapsed time since the last time update was called.
    /// @see ISystem
    void update(sf::Time dt);

    /// Add this IEntity to the given ISystem.
    /// @see ISystem
    /// @param system The system with which to register.
    /// @param callback This always be passed by default.  It is used
    /// internally.
    void addSystem(ISystem &system, bool callback = true);

    /// Remove this IEntity from the given ISystem.
    /// @see ISystem
    /// @param system The system from which to deregister.
    /// @param callback This is always passed by default, It is used internally.
    void dropSystem(ISystem &system, bool callback = true);

    /// @return The global position of this IEntity.
    sf::Vector2f getGlobalPosition() const;

    /// @return The global Axis-Aligned Boundary Box for this IEntity.
    sf::FloatRect getGlobalBounds() const;

    /// @return The global sf::Transform for this IEntity.  This will compound
    /// The local transform of the IEntity with it's parents transform, (if
    /// any).
    sf::Transform getGlobalTransform() const;

    /// @return The zOrder, (drawing order), for this IEntity.
    int zOrder() const                            { return zOrder_; }

    /// @return Set a new zOrder for this IEntity.
    void zOrder(int order)                        { zOrder_ = order; }

private:
    /// Draw all child IEntity objects associated with this IEntity.
    /// @param target Where to draw the children.
    /// @param states The sf::RenderStates to use for drawing.
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

    /// Draw this object, (and all of it's children.)
    /// This is usually called by an instance of DrawingSystem.
    /// @see DrawingSystem
    /// @param target Where to draw.
    /// @states the sf::RenderStates to use.
    void draw (sf::RenderTarget &target, sf::RenderStates states) const final;

    /// Update all of the children.  This is called by update().
    /// @param dt The elapsed time since the last update.
    void updateChildren(sf::Time dt);

    /// Handle updates for this IEntity object.  By default this is an empty
    /// function and nothing is done.  If specialized updates are desired, then
    /// this function should be overridden in a derived class.  Normally,
    /// updates are handled through MovementSystem.
    /// @see MovementSystem
    virtual void updateThis(sf::Time dt);

    /// Draw this IEntity.  This will draw ONLY this IEntity, not any children.
    /// This method does *nothing* and must be overridden in derived classes
    /// to have any effect.
    /// @param target Where to draw
    /// @states sf::RenderStates to use to draw this IEntity.
    virtual void drawThis(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    using Children = std::vector<IEntity*>;
    using Systems = std::vector<ISystem*>;

    Children children_;
    Systems systems_;
    IEntity *parent_{nullptr};
    int zOrder_{0};

public:
    /// Properties to use for this IEntity.  These are usually used by
    /// the ISystem mechanism to handle various arbitrary properties.
    /// @see MovementSystem for an example.
    PropertyManager properties;

};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_ENTITY_H
