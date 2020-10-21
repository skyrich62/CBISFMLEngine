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
 * @brief Interface for CollisionSystem
*/

#ifndef COMPUBRITE_SFML_COLLISIONSYSTEM_H
#define COMPUBRITE_SFML_COLLISIONSYSTEM_H

#include <CompuBrite/hash_util.h>
#include <CompuBrite/SFML/ISystem.h>

#include <SFML/Graphics/Rect.hpp>

#include <typeinfo.h>
#include <unordered_map>
#include <functional>

namespace CompuBrite::SFML {

/// Detect collisions between IEntity objects, and dispatch handlers to deal
/// with the detected collisions.
class CollisionSystem : public CompuBrite::SFML::ISystem
{
public:

    /// The level of collision detection to compute.
    /// Each subsequent level first checks the level before, and if no
    /// collision is found, it aborts early.  For example, If SAT is selected,
    /// then first AABB will be performed if a collision is not detected, then
    /// processing will end.  Otherwise BB will be performed, and again if
    /// no collision, processing will stop, otherwise SAT will finally be
    /// performed.
    enum Level {
        AABB,                 ///!< Axis-aligned Bounding Box
        BB,                   ///!< Bounding box
        SAT                   ///!< Separating Axis Theorem
    };

    /// Encapsulate the type_info information.  This is used to help with
    /// the collision handlers.
    using TypeIDs = std::pair<const std::type_info*, const std::type_info*>;

    /// Encapsulate the handler function.
    using Handler = std::pair<bool, std::function<void(IEntity &, IEntity&, const sf::FloatRect &)>>;

    /// Construct the CollionSystem with the given level of detection
    /// precision.
    /// @param level The requested level of precision.
    explicit CollisionSystem(Level level);
    virtual ~CollisionSystem() = default;

    /// Update the collision system.  This method is usually automatically
    /// called by the framework.  This method will detect the collisions
    /// and dispatch the handlers.
    /// @param dt The SFML time since the last call to update.
    void update(sf::Time dt) override;

    /// Add a collision handler to this system.  If a collision is detected
    /// between objects of the given types, (must be derived from IEntity),
    /// then the callback handler will be invoked.
    /// @tparam T1 The type for the first parameter of the callback function.
    /// @tparam T2 the type for the second parameter of the callback function.
    /// @param callback The callback function.
    template<typename T1, typename T2>
    void addHandler(std::function<void(T1&, T2&, const sf::FloatRect &)> callback)
    {
        auto t1 = &typeid(T1);
        auto t2 = &typeid(T2);
        auto handler = Handler(t2 < t1,
                    [callback] (IEntity& e1, IEntity &e2, const sf::FloatRect &rect)
                    {
                        Dispatcher<T1, T2> cd(callback);
                        cd(e1, e2, rect);
                    });
        auto types = std::minmax(t1, t2);
        handlers_[types] = handler;
    }

private:
    /// Utility class to assist in dispatching the collions callbacks.
    /// @tparam T1 The type of the first parameter to the callback
    /// @tparam T2 The type of the second parameter to the callback
    /// T1 and T2 must both be derived from IEntity.
    template <typename T1, typename T2>
    class Dispatcher
    {
    public:
        using Handler = std::function<void(T1 &, T2&, const sf::FloatRect &)>;

        Dispatcher(const Handler &handler) :
            handler_(handler)
        { }

        /// Dispatch the callback by safely downcasting the IEntity objects
        /// To the types provided.
        void operator()(IEntity &e1, IEntity &e2, const sf::FloatRect &rect)
        {
            handler_(static_cast<T1&>(e1), static_cast<T2&>(e2), rect);
        }
    private:
        Handler handler_;
    };

private:
    using Handlers = std::unordered_map<TypeIDs, Handler>;

    /// Determine if the two given objects have collided.
    /// @param lhs The "left hand side" object
    /// @param rhs The "right hand side" object.
    /// @param rect If a collision was detected, then rect will contain the
    /// boundary box of the detected collision.
    /// @return true if a collision was detected, false otherwise.
    bool didCollide(const IEntity &lhs, const IEntity &rhs, sf::FloatRect &rect) const;

    /// Check for and handle collisions between all IEntity objects assigned
    /// to this CollisionSystem.
    void checkCollisions() const;

    /// A collision was detected between two objects, handle the collision by
    /// dispatching the correct callback.
    /// @param lhs The "left hand side" of the collision.
    /// @param rhs The "Right hand side" of the collision.
    /// @param rect The boundary box containing the point of collision.
    void handleCollision(IEntity &lhs, IEntity &rhs, const sf::FloatRect &rect) const;

    /// Find a handler for the given types.
    /// @param p A std::pair of type_info pointers.  If a handler has been
    /// registeredd for the given types, return it.
    /// @return the given handler if it exists, otherwise return an empty
    /// std::function
    Handler find(const TypeIDs &p) const;

    Handlers handlers_;
    Level level_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_COLLISIONSYSTEM_H
