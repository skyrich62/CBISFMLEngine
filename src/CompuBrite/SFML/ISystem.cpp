#include "CompuBrite/SFML/ISystem.h"
#include "CompuBrite/SFML/IEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite {
namespace SFML {

ISystem::ISystem()
{
    //ctor
}

ISystem::~ISystem()
{
    //dtor
}

void
ISystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}

void
ISystem::update(sf::Time dt)
{
}

void
ISystem::addEntity(IEntity &entity, bool callback)
{
    auto found = std::find(entities_.begin(), entities_.end(), &entity);
    if (found == entities_.end()) {
        addProperties(entity);
        entities_.push_back(&entity);
        if (callback) {
            entity.addSystem(*this, false);
        }
    }
}

void
ISystem::dropEntity(IEntity &entity, bool callback)
{
    auto found = std::find(entities_.begin(), entities_.end(), &entity);
    if (found != entities_.end()) {
        entities_.erase(found);
        if (callback) {
            entity.dropSystem(*this, false);
        }
    }
}

void
ISystem::addProperties(IEntity &)
{
}

} // namespace SFML
} // namespace CompuBrite
