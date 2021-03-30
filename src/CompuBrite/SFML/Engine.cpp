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
 * @brief Implementation for Engine
*/

#include "CompuBrite/SFML/Engine.h"
#include "CompuBrite/SFML/Context.h"

#include "SFML/System/Time.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Sleep.hpp"


namespace CompuBrite::SFML {

Context&
Engine::addContext(const std::string &name,
            StateStack &&stack,
            sf::Time timeSlice,
            sf::VideoMode mode,
            const sf::String &title,
            sf::Uint32 style,
            const sf::ContextSettings &settings)
{
    _contexts.enroll<Context>(name, name, *this, std::move(stack), timeSlice);
    auto &context = _contexts.get(name);
    context.setArgs(name, mode, title, style, settings);
    return context;
}

Context&
Engine::addContext(const std::string &name,
            sf::Time timeSlice,
            sf::VideoMode mode,
            const sf::String &title,
            sf::Uint32 style,
            const sf::ContextSettings &settings)
{
    _contexts.enroll<Context>(name, name, *this, StateStack(), timeSlice);
    auto &context = _contexts.get(name);
    context.setArgs(name, mode, title, style, settings);
    return context;
}

void
Engine::run()
{
    for (auto &context : _contexts) {
        context.second->start();
    }
    sf::sleep(sf::seconds(1.0f));

    while (true) {
        auto running = false;
        for (auto &context : _contexts) {
            running |= context.second->isRunning();
        }
        if (!running) {
            break;
        }
    }
}

} // namespace CompuBrite::SFML
