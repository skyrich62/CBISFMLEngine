#include "CompuBrite/SFML/CircleEntity.h"

namespace CompuBrite {
namespace SFML {

CircleEntity::CircleEntity(float radius, std::size_t pointCount) :
    IShapeEntity(circle_),
    circle_(radius, pointCount)
{
}

void
CircleEntity::setRadius(float radius)
{
    circle_.setRadius(radius);
}

float
CircleEntity::getRadius() const
{
    circle_.getRadius();
}

void
CircleEntity::setPointCount(std::size_t count)
{
    circle_.setPointCount(count);
}

} // namespace SFML
} // namespace CompuBrite
