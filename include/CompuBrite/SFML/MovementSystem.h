#ifndef COMPUBRITE_SFML_MOVEMENTSYSTEM_H
#define COMPUBRITE_SFML_MOVEMENTSYSTEM_H

#include <CompuBrite/SFML/ISystem.h>

namespace CompuBrite
{
namespace SFML
{

class MovementSystem : public CompuBrite::SFML::ISystem
{
public:
    MovementSystem() = default;
    virtual ~MovementSystem() = default;
    void update(sf::Time dt) override;

private:
    void addProperties(IEntity &entity) override;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_MOVEMENTSYSTEM_H
