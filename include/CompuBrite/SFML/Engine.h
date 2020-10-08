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
 * @brief Interface for Engine
*/

#ifndef COMPUBRITE_SFML_ENGINE_H
#define COMPUBRITE_SFML_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <CompuBrite/SFML/EventManager.h>
#include <CompuBrite/SFML/StateStack.h>

namespace CompuBrite
{
namespace SFML
{

/// Encapsulates the main interface for the framework.  The Engine handles
/// events, (keyboard, mouse, joystick, etc), holds the state stack, and
/// manages updating and drawing all objects through the states.
class Engine
{
public:
    Engine() = default;
    virtual ~Engine() = default;

    /// Run this Engine using the given target.
    /// @param target Where to draw all the IEntity objects.
    void run(sf::RenderWindow &target);

    /// Add a handler for an event.  If the given event is detected, then
    /// dispatch to the given callback handler.
    /// @param event The sf::Event object to detect.
    /// @param command The handler callback for that event.
    void addEvent(const sf::Event &event, EventManager::Command command);

    /// @return A reference to the managed StateStack for use by the
    /// application.
    StateStack &stack()                      { return stack_; }
private:
    EventManager events_;
    StateStack   stack_;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENGINE_H
