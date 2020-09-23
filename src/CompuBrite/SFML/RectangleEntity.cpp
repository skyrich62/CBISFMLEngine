#include "CompuBrite/SFML/RectangleEntity.h"

namespace CompuBrite {
namespace SFML {

RectangleEntity::RectangleEntity(const sf::Vector2f &size) :
    IShapeEntity(rectangle_),
    rectangle_(size)
{
}

void
RectangleEntity::setSize(const sf::Vector2f &size)
{
    rectangle_.setSize(size);
}

const sf::Vector2f &
RectangleEntity::getSize() const
{
    return rectangle_.getSize();
}

} // namespace SFML
} // namespace CompuBrite
