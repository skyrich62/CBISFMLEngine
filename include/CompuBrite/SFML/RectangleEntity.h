#ifndef COMPUBRITE_SFML_RECTANGLEENTITY_H
#define COMPUBRITE_SFML_RECTANGLEENTITY_H

#include <CompuBrite/SFML/IShapeEntity.h>
#include <SFML/Graphics/RectangleShape.hpp>

namespace CompuBrite
{
namespace SFML
{

class RectangleEntity : public CompuBrite::SFML::IShapeEntity
{
public:
    RectangleEntity(const sf::Vector2f &size = sf::Vector2f(0.0f, 0.0f));

    void setSize(const sf::Vector2f &size);
    const sf::Vector2f &getSize() const;

protected:

private:
    sf::RectangleShape rectangle_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_RECTANGLEENTITY_H
