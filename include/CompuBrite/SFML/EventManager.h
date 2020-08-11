#ifndef COMPUBRITE_SFML_EVENTMANAGER_H
#define COMPUBRITE_SFML_EVENTMANAGER_H

#include <map>
#include <functional>

#include <SFML/Window/Event.hpp>

namespace CompuBrite
{
namespace SFML
{

struct EventLess
{
    bool operator()(const sf::Event &lhs, const sf::Event &rhs);
};

class EventManager
{
public:
    using Command = std::function<void(const sf::Event&)>;

    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager(EventManager&&) = default;
    ~EventManager() = default;

    EventManager& operator=(const EventManager&) = delete;
    EventManager& operator=(EventManager&&) = default;

    void add(const sf::Event &, Command);

    void dispatch(const sf::Event &event);
private:
    Command find(const sf::Event &event);

private:
    using Events = std::map<sf::Event, Command, EventLess>;
    Events events_;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_EVENTMANAGER_H
