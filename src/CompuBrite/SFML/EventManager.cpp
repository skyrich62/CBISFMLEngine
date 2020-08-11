#include "CompuBrite/SFML/EventManager.h"
#include "compubrite/checkpoint.h"

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
