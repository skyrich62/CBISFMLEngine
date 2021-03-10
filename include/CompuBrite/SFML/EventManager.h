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
 * @brief Interface for EventManager
*/

#ifndef COMPUBRITE_SFML_EVENTMANAGER_H
#define COMPUBRITE_SFML_EVENTMANAGER_H

#include <map>
#include <functional>

#include <SFML/Window/Event.hpp>

namespace CompuBrite::SFML {

class StateStack;
class Context;

/// Used to compare two events, (useful for std::map).
struct EventLess
{
    bool operator()(const sf::Event &lhs, const sf::Event &rhs);
};

/// Manage events by registering sf::Event objects against a handler callback
/// function.
class EventManager
{
public:
    using Command = std::function<void(const sf::Event&, Context &)>;

    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager(EventManager&&) = default;
    ~EventManager() = default;

    EventManager& operator=(const EventManager&) = delete;
    EventManager& operator=(EventManager&&) = default;

    /// Add a handler for the given event.
    /// @param event the sf::Event object to detect.
    /// @param command the handler callback function.
    void add(const sf::Event &event, Command command);

    /// Dispatch the callback for the given event, if it exists.
    /// @param event The event which was detected.
    /// @param stack The StateStack or nullptr
    void dispatch(const sf::Event &event, Context &context);
private:
    /// Find a callback for the given event.
    /// @param event The event which was detected.
    /// @return A callback hander for this event if there is one, an empty
    /// std::function if not.
    Command find(const sf::Event &event);

private:
    using Events = std::map<sf::Event, Command, EventLess>;
    Events events_;

};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_EVENTMANAGER_H
