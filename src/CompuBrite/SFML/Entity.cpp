#include "CompuBrite/SFML/Entity.h"

namespace CompuBrite {
namespace SFML {

Entity::~Entity()
{
    //dtor
}

sf::FloatRect
Entity::getLocalBounds() const
{
    return sf::FloatRect();
}

void
Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    drawThis(target, states);
    drawChildren(target, states);
}

void
Entity::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto &level : children_) {
        for (auto &child : level.second) {
            child->draw(target, states);
        }
    }
}

bool
Entity::addChild(Entity &child, int zOrder)
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
Entity::drawThis(sf::RenderTarget &, sf::RenderStates) const
{
}

void
Entity::update(sf::Time dt)
{
    updateThis(dt);
    updateChildren(dt);
}

void
Entity::updateChildren(sf::Time dt)
{
    for (auto &level : children_) {
        for (auto &child : level.second) {
            child->update(dt);
        }
    }
}

void
Entity::updateThis(sf::Time dt)
{
}

} // namespace SFML
} // namespace CompuBrite
