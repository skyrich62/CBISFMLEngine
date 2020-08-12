#include "CompuBrite/SFML/IEntity.h"
#include "CompuBrite/SFML/ISystem.h"

namespace CompuBrite {
namespace SFML {

IEntity::~IEntity()
{
    //dtor
}

sf::FloatRect
IEntity::getLocalBounds() const
{
    return sf::FloatRect();
}

void
IEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    drawThis(target, states);
    drawChildren(target, states);
}

void
IEntity::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &level : children_) {
        for (auto &child : level.second) {
            child->draw(target, states);
        }
    }
}

bool
IEntity::addChild(IEntity &child, int zOrder)
{
    child.zOrder_ = zOrder;
    auto found = children_.find(zOrder);
    if (found == children_.end()) {
        children_.emplace(zOrder, zLevel());
        found = children_.find(zOrder);
    }
    auto &vec = found->second;
    if (std::find(vec.begin(), vec.end(), &child) == vec.end()) {
        vec.emplace_back(&child);
        return true;
    }
    return false;
}

void
IEntity::drawThis(sf::RenderTarget &, sf::RenderStates) const
{
}

void
IEntity::update(sf::Time dt)
{
    updateThis(dt);
    updateChildren(dt);
}

void
IEntity::updateChildren(sf::Time dt)
{
    for (auto &level : children_) {
        for (auto &child : level.second) {
            child->update(dt);
        }
    }
}

void
IEntity::updateThis(sf::Time dt)
{
}

void
IEntity::addSystem(ISystem &system, bool callback)
{
    auto found = find(systems_.begin(), systems_.end(), &system);
    if (found == systems_.end()) {
        systems_.push_back(&system);
        if (callback) {
            system.addEntity(*this, false);
        }
    }
}

void
IEntity::dropSystem(ISystem &system, bool callback)
{
    auto found = find(systems_.begin(), systems_.end(), &system);
    if (found != systems_.end()) {
        systems_.erase(found);
        if (callback) {
            system.dropEntity(*this, false);
        }
    }
}

} // namespace SFML
} // namespace CompuBrite
