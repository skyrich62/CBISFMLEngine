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
 * @brief Interface for State
*/

#ifndef COMPUBRITE_SFML_STATE_H
#define COMPUBRITE_SFML_STATE_H

#include <vector>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>


namespace CompuBrite
{
namespace SFML
{
class ISystem;

/// Used to provide state information to the application.  The Engine object
/// holds a StateStack object, which holds states.  The Engine will call
/// StateStack::draw(), and StateStack::update() at appropriate times.
/// StateStack will in turn call State::draw() and State::update() for the
/// State objects that it manages.
/// @see StateStack
class State
{
public:
    State() = default;
    /// Construct a State specifying whether processing should proceed or
    /// stop.
    /// @param lastDrawn true if this should be the last drawn state in the stack.
    /// @param latUpdated true if this should be the last updated state.
    State(bool lastDrawn, bool lastUpdated);

    virtual ~State() = default;

    /// Draw this state.  By default, it calls ISystem::draw() for all systems
    /// registered to this State.
    virtual bool draw(sf::RenderTarget &target, sf::RenderStates states) const;

    /// Update this state.  By default, it calls ISystem::update() for all
    /// systems registered to this State.
    virtual bool update(sf::Time dt);

    /// Add an ISystem to this state.
    void addSystem(ISystem &system);

    /// Drop an ISystem from this state.
    void dropSystem(ISystem &system);

    /// Change lastDrawn
    /// @param b True if this should be the last drawn state in its stack.
    void lastDrawn(bool b)                  { lastDrawn_ = b; }

    /// Change lastUpdated
    /// @param b True if this should be the last updated state in its stack.
    void lastUpdated(bool b)                { lastUpdated_ = b; }

private:
    std::vector<ISystem*> systems_;
    bool                  lastDrawn_ = false;
    bool                  lastUpdated_ = false;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_STATE_H
