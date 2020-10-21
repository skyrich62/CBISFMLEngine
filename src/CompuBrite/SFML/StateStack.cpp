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
 * @brief Implementation for StateStack
*/

#include "CompuBrite/SFML/State.h"
#include "CompuBrite/SFML/StateStack.h"

#include <iostream>

namespace CompuBrite::SFML {

void
StateStack::push(State &state)
{
    states_.push_back(&state);
}

State*
StateStack::top() const
{
    if (states_.empty()) {
        return nullptr;
    }
    return states_.back();
}

State*
StateStack::pop()
{
    auto ret = top();
    if (ret) {
        states_.pop_back();
    }
    return ret;
}

void
StateStack::clear()
{
    states_.clear();
}

void
StateStack::dispatch(const sf::Event &event)
{
    std::vector<State*> temp(states_.rbegin(), states_.rend());
    for (auto state: temp) {
        auto stop = state->dispatch(event);
        if (stop) {
            break;
        }
    }
}

void
StateStack::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::vector<State*> temp(states_.begin(), states_.end());
    for (auto state: temp) {
        auto stop = state->draw(target, states);
        if (stop) {
            break;
        }
    }
}

void
StateStack::update(sf::Time dt)
{
    std::vector<State*> temp(states_.rbegin(), states_.rend());
    for (auto state: temp) {
        auto stop = state->update(dt);
        if (stop) {
            break;
        }
    }
}

} // namespace CompuBrite::SFML
