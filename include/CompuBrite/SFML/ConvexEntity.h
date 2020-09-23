#ifndef COMPUBRITE_SFML_CONVEXENTITY_H
#define COMPUBRITE_SFML_CONVEXENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/ConvexShape.hpp>

namespace CompuBrite
{
namespace SFML
{

class ConvexEntity : public IShapeEntity
{
public:
    explicit ConvexEntity(std::size_t pointCount = 0);

    void setPointCount(std::size_t count);
    void setPoint(std::size_t index, const sf::Vector2f &point);

protected:

private:
    sf::ConvexShape shape_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_CONVEXENTITY_H
