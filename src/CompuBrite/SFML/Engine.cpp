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
