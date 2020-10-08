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

#include "SFML/System/Time.hpp"
#include "SFML/System/Clock.hpp"

namespace CompuBrite {
namespace SFML {

void
Engine::run(sf::RenderWindow &target)
{
    if (stack().empty()) {
        return;
    }
    sf::Clock clock;
	// Start the game loop
    while (target.isOpen())
    {
        auto elapsed = clock.restart();
        // Process events
        sf::Event event;
        while (target.pollEvent(event))
        {
            events_.dispatch(event);
        }

        // Clear screen
        target.clear();

        target.draw(stack_);
        stack_.update(elapsed);

        // Update the window
        target.display();
    }

}

void
Engine::addEvent(const sf::Event &event, EventManager::Command command)
{
    events_.add(event, command);
}

} // namespace SFML
} // namespace CompuBrite
