#include "CompuBrite/SFML/State.h"
#include "CompuBrite/SFML/StateStack.h"

namespace CompuBrite {
namespace SFML {

void
StateStack::push(State &state)
{
    states_.push_back(&state);
}

State*
StateStack::top() const
{
    auto size = states_.size();
    if (size > 0u) {
        return states_[size -1];
    }
    return nullptr;
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
StateStack::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::vector<State*> temp(states_.rbegin(), states_.rend());
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

} // namespace SFML
} // namespace CompuBrite
