#include "CompuBrite/SFML/Entity.h"

namespace CompuBrite {
namespace SFML {

Entity::Entity(EntityImpl impl) :
    impl_(impl)
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void
Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    impl_.drawThis_(target, states);
    drawChildren(target, states);
}

void
Entity::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto child : children_) {
        child->draw(target, states);
    }
}

} // namespace SFML
} // namespace CompuBrite
