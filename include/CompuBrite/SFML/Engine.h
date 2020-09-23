#ifndef COMPUBRITE_SFML_ENGINE_H
#define COMPUBRITE_SFML_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <CompuBrite/SFML/EventManager.h>
#include <CompuBrite/SFML/StateStack.h>

namespace CompuBrite
{
namespace SFML
{

class Engine
{
public:
    Engine() = default;
    virtual ~Engine() = default;
    void run(sf::RenderWindow &target);

    void addEvent(const sf::Event &event, EventManager::Command command);
    StateStack &stack()                      { return stack_; }
private:
    EventManager events_;
    StateStack   stack_;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENGINE_H
