#include "CompuBrite/SFML/ConvexEntity.h"

namespace CompuBrite {
namespace SFML {

ConvexEntity::ConvexEntity(std::size_t pointCount) :
    IShapeEntity(shape_),
    shape_(pointCount)
{
}

void
ConvexEntity::setPointCount(std::size_t count)
{
    shape_.setPointCount(count);
}

void
ConvexEntity::setPoint(std::size_t index, const sf::Vector2f &point)
{
    shape_.setPoint(index, point);
}

} // namespace SFML
} // namespace CompuBrite
