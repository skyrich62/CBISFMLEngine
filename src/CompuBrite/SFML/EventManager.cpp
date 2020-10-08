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
 * @brief Implementation for EventManager
*/

#include "CompuBrite/SFML/EventManager.h"
#include "CompuBrite/CheckPoint.h"

namespace CompuBrite {
namespace SFML {

bool
EventLess::operator()(const sf::Event &lhs, const sf::Event &rhs)
{
    if (lhs.type == rhs.type) {
        switch (lhs.type) {
        case sf::Event::KeyReleased:
        case sf::Event::KeyPressed:
            return lhs.key.code < rhs.key.code;
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            return lhs.mouseButton.button == rhs.mouseButton.button;
        default:
            CompuBrite::CheckPoint::hit(CBI_HERE, "Unexpected event");
            break;
        }
        return false;
    }
    return lhs.type < rhs.type;
}

void
EventManager::add(const sf::Event &event, Command command)
{
    events_[event] = command;
}

EventManager::Command
EventManager::find(const sf::Event &event)
{
    if (auto found = events_.find(event); found != events_.end()) {
        return found->second;
    }
    return Command();
}

void
EventManager::dispatch(const sf::Event &event)
{
    auto command = find(event);
    if (command) {
        command(event);
    }
}

} // namespace SFML
} // namespace CompuBrite
