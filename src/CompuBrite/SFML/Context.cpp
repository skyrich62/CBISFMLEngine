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
    if (true) {
        Lock lock(_mutex);
        _started.wait(lock);
    }
    while (_running && _window.isOpen()) {
        sf::sleep(_timeSlice / 4.0f);
        _elapsed += _clock.restart();
        while (_elapsed > _timeSlice) {
            _elapsed -= _timeSlice;
            _stack.update(_timeSlice, *this);
        }
    }
}

void
Context::render()
{
    if (true) {
        Lock lock(_mutex);
        _started.wait(lock);
    }
    while (_running && _window.isOpen()) {
        _window.clear();
        _window.draw(_stack);
        _window.display();
        _window.setActive(false);
        sf::sleep(_timeSlice);
    }
}

void
Context::run()
{
    Lock lock(_mutex);
    auto &[name, mode, title, style, settings] = _windowArgs;
    _window.create(mode, title, style, settings);
    _window.setActive(false);
    _started.notify_all();
    lock.unlock();
    sf::Event event;
    while (_running) {
        if (_window.waitEvent(event)) {
            _stack.dispatch(event, *this);
            _events.dispatch(event, *this);
            if (event.type == sf::Event::Closed || _stack.empty()) {
                _window.close();
                _running = false;
                _window.setActive(false);
                return;
            }
        }
    }
}

void
Context::start()
{
    if (true) {
        Lock lock(_mutex);

        _running = true;
        _engine.addTask([this] { this->run(); });
        _engine.addTask([this] { this->update(); });
        _engine.addTask([this] { this->render(); });
    }
    _engine.activate(3);
}

void
Context::addEvent(const sf::Event &event, EventManager::Command command)
{
    _events.add(event, command);
}


} // namespace CompuBrite

// EOF
