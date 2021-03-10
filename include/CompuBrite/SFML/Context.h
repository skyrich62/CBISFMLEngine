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
 * @brief Interface for Context
*/

#ifndef COMPUBRITE_SFML_CONTEXT_H
#define COMPUBRITE_SFML_CONTEXT_H

#include <string>
#include <SFML/System/Time.hpp>
#include <CompuBrite/SFML/StateStack.h>

namespace CompuBrite::SFML {
class Engine;

/**
 * A Context holds an sfml window, a timeslice and a state stack.  This is
 * used to run and manage the window.
 */
class Context
{
public:
    Context(const std::string &name,
            Engine &engine,
            StateStack &&stack,
            sf::Time timeSlice,
            sf::VideoMode mode,
            const sf::String &title,
            sf::Uint32 style,
            const sf::ContextSettings &settings) :
        _window(mode, title, style, settings),
        _stack(std::move(stack)),
        _name(name),
        _timeSlice(timeSlice),
        _engine(engine)
    { }

    Context(const std::string &name,
            Engine & engine,
            StateStack &&stack,
            sf::Time timeSlice,
            sf::VideoMode mode,
            const sf::String &title) :
        _window(mode, title),
        _stack(std::move(stack)),
        _name(name),
        _timeSlice(timeSlice),
        _engine(engine)
    { }

    Context(const std::string &name,
            Engine &engine,
            StateStack &&stack,
            sf::Time timeSlice) :
        _stack(std::move(stack)),
        _name(name),
        _timeSlice(timeSlice),
        _engine(engine)
    { }
    auto &window()                               { return _window; }
    const auto& window() const                   { return _window; }
    auto &stack()                                { return _stack;  }
    const auto &stack() const                    { return _stack;  }

    auto slice() const                           { return _timeSlice; }
    auto elapsed() const                         { return _elapsed; }
    const auto &name() const                     { return _name; }

    /// Add a handler for an event.  If the given event is detected, then
    /// dispatch to the given callback handler.
    /// @param event The sf::Event object to detect.
    /// @param command The handler callback for that event.
    void addEvent(const sf::Event &event, EventManager::Command command);

    void run();

private:
    void update();
    void render();
    void processEvents();

private:
    EventManager     _events;
    sf::RenderWindow _window;
    StateStack       _stack;
    sf::Clock        _clock;
    std::string      _name;
    sf::Time         _timeSlice;
    sf::Time         _elapsed;
    Engine          &_engine;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_CONTEXT_H
