#include "CompuBrite/SFML/Engine.h"

#include "SFML/System/Time.hpp"
#include "SFML/System/Clock.hpp"

namespace CompuBrite {
namespace SFML {

void
Engine::run(sf::RenderWindow &target)
{
    sf::Clock clock;
	// Start the game loop
    while (target.isOpen())
    {
        // Process events
        sf::Event event;
        while (target.pollEvent(event))
        {
            events_.dispatch(event);
        }

        // Clear screen
        target.clear();

        for (auto system : systems_) {
            system->update(clock.restart());
            target.draw(*system);
        }

        // Update the window
        target.display();
    }

}

void
Engine::addEvent(const sf::Event &event, EventManager::Command command)
{
    events_.add(event, command);
}

void
Engine::addSystem(ISystem &system)
{
    systems_.push_back(&system);
}

} // namespace SFML
} // namespace CompuBrite
