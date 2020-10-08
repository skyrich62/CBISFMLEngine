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
 * @brief Implementation for State
*/

#include "CompuBrite/SFML/State.h"
#include "CompuBrite/SFML/ISystem.h"

namespace CompuBrite {
namespace SFML {

State::State(bool lastDrawn, bool lastUpdated) :
    lastDrawn_(lastDrawn),
    lastUpdated_(lastUpdated)
{
}

bool
State::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto system: systems_) {
        system->draw(target, states);
    }
    return lastDrawn_;
}

bool
State::update(sf::Time dt)
{
    for (auto system: systems_) {
        system->update(dt);
    }
    return lastUpdated_;
}

void
State::addSystem(ISystem &system)
{
    systems_.push_back(&system);
}

void
State::dropSystem(ISystem &system)
{
    auto found = std::find(systems_.begin(), systems_.end(), &system);
    if (found == systems_.end()) {
        return;
    }
    systems_.erase(found);
}

} // namespace SFML
} // namespace CompuBrite
