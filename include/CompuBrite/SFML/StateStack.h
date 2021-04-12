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
 * @brief Interface for StateStack
*/

#ifndef COMPUBRITE_SFML_STATESTACK_H
#define COMPUBRITE_SFML_STATESTACK_H

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include <CompuBrite/SFML/Engine.h>

namespace CompuBrite::SFML {

class State;
class ISystem;
class Context;
class Engine;

/// Manage a stack of State objects.  The Engine object will use this stack
/// to manage all updating and drawing of all objects in the application. If
/// the stack is empty, then Engine::run() will return. This is a convenient
/// way to end the application.
class StateStack
{
public:
    StateStack() = default;
    ~StateStack() = default;

    /// Push the given state onto the stack.
    void push(State &state);

    /// @return A pointer to the state on the top of the stack.
    /// @return nullptr if the stack is empty.
    State *top() const;

    /// Pop the top element off the stack.
    /// @return A pointer to the element which was popped or nullptr if the
    /// stack was empty.
    State *pop();

    /// Clear the stack.  make it empty.
    void clear();

    /// @return true if the stack is empty.
    bool empty()                            { return states_.empty(); }

    /// call State::draw() for each element of the stack, starting at the bottom.
    /// If any State::draw() function returns true, drawing will stop after
    /// drawing that State.
    /// @see State
    void draw(Context &target, sf::RenderStates states = sf::RenderStates()) const;

    /// call State::update() for each element of the stack, starting at the top.
    /// If any State::update() function return true, updating will stop after
    /// updating that State.  For example, this is useful for "Pause" States.
    /// Where a Semi-transparent pause screen overlays the underlying states.
    /// We want the pause state to update, but not the rest of the states.
    void update(sf::Time dt, Context &context);

    /// Dispatch an event through the stack.
    /// @param event The event to dispatch
    void dispatch(const sf::Event &event, Context &);

private:
    void deactivate();
    void activate();

    std::vector<State *> states_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_STATESTACK_H
