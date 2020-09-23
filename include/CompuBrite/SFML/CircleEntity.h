#ifndef COMPUBRITE_SMFL_CIRCLEENTITY_H
#define COMPUBRITE_SMFL_CIRCLEENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/CircleShape.hpp>

namespace CompuBrite
{
namespace SFML
{

class CircleEntity : public CompuBrite::SFML::IShapeEntity
{
public:
    explicit CircleEntity(float radius = 0.f, std::size_t pointCount = 30);

    void setRadius(float radius);
    float getRadius() const;

    void setPointCount(std::size_t count);

protected:

private:
    sf::CircleShape circle_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SMFL_CIRCLEENTITY_H
