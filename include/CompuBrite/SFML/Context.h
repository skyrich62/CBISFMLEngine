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

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>


namespace CompuBrite::SFML {
class Engine;

/**
 * A Context holds an sfml window, a timeslice and a state stack.  This is
 * used to run and manage the window.
 */
class Context
{
public:

    /**
     * Construct a Context.  The Constructor should only be called from
     * Engine::addContext.
     * @param name The name for this Context. This is not the same as the
     * window title.  Instead, it's a resource identifier.  (See ResourceManager).
     * @param engine A reference to the Engine that is creating this Context.
     * @param stack The stack that this Context should use for updating and
     * drawing.  See (StateStack).
     * @param timeSlice The requested frame rate.
     */
    Context(const std::string &name,
            Engine &engine,
            StateStack &&stack,
            sf::Time timeSlice) :
        _stack(std::move(stack)),
        _name(name),
        _timeSlice(timeSlice),
        _engine(engine)
    { }

    ~Context() = default;

    /// @name Accessors
    /// @{

    /// @return a reference to the sf::RenderWindow object.
    auto &window()                               { return _window; }

    /// @return a const reference to the sf::RenderWindow object.
    const auto& window() const                   { return _window; }

    /// @return a reference to the StateStack used by this Context.
    auto &stack()                                { return _stack;  }

    /// @return a const reference to the StateStack used by this Context.
    const auto &stack() const                    { return _stack;  }

    /// @return the desired frame rate.
    auto slice() const                           { return _timeSlice; }

    /// @return The elapsed time since the last update.
    auto elapsed() const                         { return _elapsed; }

    /// @return The name of this context.
    const auto &name() const                     { return _name; }

    /// If this Context is still running, return true, otherwise false.
    bool isRunning() const                       { return _running; }

    /// @}

    /// Add a handler for an event.  If the given event is detected, then
    /// dispatch to the given callback handler.
    /// @param event The sf::Event object to detect.
    /// @param command The handler callback for that event.
    /// @see EventManager
    void addEvent(const sf::Event &event, EventManager::Command command);


    /// Activate and run this context.
    /// Start three threads in the Engine ThreadPool.
    /// 1. Create the window and process sf::Event objects.
    /// 2. Render the window.
    /// 3. Update the window stack on a regular, (_timeSlice) basis.
    void start();

    /// Save the arguments for sf::RenderWindow::create in a tuple for later
    /// use.
    void setArgs(const std::string &name,
                 sf::VideoMode mode,
                 const sf::String &title,
                 sf::Uint32 style,
                 const sf::ContextSettings &settings)
    {
        _windowArgs = {name, mode, title, style, settings};
    }



private:
    /// Update the stack on a regular, (_timeSlice) basis
    void update();

    /// Render the sf::RenderWindow on a regular, (_timeSlice) basis.
    void render();

    /// Create the sf::RenderWindow and process incoming Events from it.
    void run();

private:
    using WindowArgs = std::tuple<std::string, sf::VideoMode, sf::String, sf::Uint32, sf::ContextSettings>;
    using Mutex = std::mutex;
    using Lock = std::unique_lock<Mutex>;
    using Flag = std::atomic<bool>;
    using Cond = std::condition_variable;

    WindowArgs       _windowArgs;

    mutable Mutex    _mutex;
    mutable Flag     _running{false};
    Cond             _started;
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
