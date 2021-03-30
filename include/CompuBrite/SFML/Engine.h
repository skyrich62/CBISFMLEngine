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
#include <CompuBrite/SFML/ResourceManager.h>
#include <CompuBrite/SFML/StateStack.h>

#include <CompuBrite/ThreadPool.h>

#include <tuple>
#include <vector>

namespace CompuBrite::SFML {

class Context;

/// Encapsulates the main interface for the framework.  The Engine handles
/// events, (keyboard, mouse, joystick, etc), holds the state stack, and
/// manages updating and drawing all objects through the states.
class Engine : public ThreadPool
{
public:
    using Task = ThreadPool::Task;

    Engine() = default;
    virtual ~Engine() = default;

    /// Add a window context.  This will not create the window, that happens
    /// later.  Instead this sets up the Context under which the window
    /// will be created.
    /// @param name The name of this context
    /// @param stack The stack which this context should subsume.
    /// @param timeSlice The requested frame rate for this window.
    /// @param mode The video modes to be passed to SFML
    /// @param title The title of the window to be passed to SFML
    /// @param style The window style bitmap to be passed to SFML
    /// @param settings The OpenGL context settings to be passed to SFML.
    Context& addContext(const std::string &name,
             StateStack &&stack,
             sf::Time timeSlice,
             sf::VideoMode mode,
             const sf::String &title,
             sf::Uint32 style = sf::Style::Default,
             const sf::ContextSettings &settings = sf::ContextSettings());

    /// @override
    /// This override doesn't supply a stack, so a new stack is created
    /// automatically.  All other parameters are the same.
    Context& addContext(const std::string &name,
             sf::Time timeSlice,
             sf::VideoMode mode,
             const sf::String &title,
             sf::Uint32 style = sf::Style::Default,
             const sf::ContextSettings &settings = sf::ContextSettings());

    /// Get the context of the referenced name.
    /// After a context has been created, it can be retrieved by name.
    /// @param name The name of the desired context.  This must match
    /// the "name" parameter of a call to addContext.
    /// @return A reference to the desired context.
    /// @note If there is no such context, a reference to a null pointer will
    /// be returned.  Dereferencing it will cause a crash.
    Context& getContext(const std::string name)  { return _contexts.get(name); }

    /// Activate and run all associated Contexts.  This function will return
    /// only after all Contexts have stopped running.
    void run();

private:
    ResourceManager<std::string, Context> _contexts;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_ENGINE_H
