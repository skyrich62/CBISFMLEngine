#include "CompuBrite/SFML/MovementSystem.h"

#include <SFML/System/Vector2.hpp>

namespace CompuBrite {
namespace SFML {

void
MovementSystem::update(sf::Time dt)
{
    for (auto entity: entities_) {
        auto &vel = entity->properties.ref<sf::Vector2f>("velocity");
        auto &rot = entity->properties.ref<float>("rotation");

        auto acc = entity->properties.get<sf::Vector2f>("acceleration");
        auto racc = entity->properties.get<float>("rot_accel");

        auto t = dt.asSeconds();

        vel += acc * t;
        rot += racc * t;

        entity->move(vel * t);
        entity->rotate(rot * t);
    }
}

void
MovementSystem::addProperties(IEntity &entity)
{
    entity.properties.add<sf::Vector2f>("velocity");
    entity.properties.add<float>("rotation");
    entity.properties.add<sf::Vector2f>("acceleration");
    entity.properties.add<float>("rot_accel");
}

} // namespace SFML
} // namespace CompuBrite
