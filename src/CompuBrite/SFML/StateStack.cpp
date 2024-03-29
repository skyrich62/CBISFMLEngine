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
    deactivate();
    states_.push_back(&state);
    state.onPush(*this);
    activate();
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
    deactivate();
    auto ret = top();
    if (ret) {
        states_.pop_back();
        ret->onPop();
    }
    activate();
    return ret;
}

void
StateStack::clear()
{
    deactivate();
    for (auto state: states_) {
        state->onPop();
    }
    states_.clear();
}

void
StateStack::dispatch(const sf::Event &event, Context &context)
{
    std::vector<State*> temp(states_.rbegin(), states_.rend());
    for (auto state: temp) {
        auto stop = state->dispatch(event, context);
        if (stop) {
            break;
        }
    }
}

void
StateStack::draw(Context &target, sf::RenderStates states) const
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
StateStack::update(sf::Time dt, Context &context)
{
    std::vector<State*> temp(states_.rbegin(), states_.rend());
    for (auto state: temp) {
        auto stop = state->update(dt, context);
        if (stop) {
            break;
        }
    }
}

void
StateStack::activate()
{
    if (auto state = top(); state) {
        state->onActivation();
    }
}

void
StateStack::deactivate()
{
    if (auto state = top(); state) {
        state->onDeactivation();
    }
}

} // namespace CompuBrite::SFML
