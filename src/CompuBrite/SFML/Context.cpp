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
 * @brief Implementation for Context.
*/

#include <SFML/System/Sleep.hpp>
#include "CompuBrite/SFML/Context.h"

namespace CompuBrite:: SFML {

void
Context::update()
{
    processEvents();
    _elapsed += _clock.restart();
    while (_elapsed > _timeSlice) {
        _elapsed -= _timeSlice;
        _stack.update(_timeSlice, *this);
    }
}

void
Context::render()
{
    _window.clear();
    _window.draw(_stack);
    _window.display();
}

void
Context::processEvents()
{
    sf::Event event;
    while (_window.pollEvent(event)) {
        stack().dispatch(event, *this);
        _events.dispatch(event, *this);
    }
}

void
Context::run()
{
    _elapsed = sf::Time::Zero;
    while (_window.isOpen()) {
        if (stack().empty()) {
            _window.close();
            return;
        }
        update();
        render();
        sf::sleep((_timeSlice - _elapsed) / 4.0f);
    }
}

void
Context::addEvent(const sf::Event &event, EventManager::Command command)
{
    _events.add(event, command);
}


} // namespace CompuBrite

// EOF
