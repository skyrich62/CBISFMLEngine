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
