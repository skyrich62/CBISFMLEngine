#ifndef COMPUBRITE_SFML_ISYSTEM_H
#define COMPUBRITE_SFML_ISYSTEM_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <CompuBrite/SFML/IEntity.h>

#include <vector>

namespace CompuBrite
{
namespace SFML
{

class ISystem;

class ISystem : public sf::Drawable
{
public:
    ISystem();
    virtual ~ISystem();

    void addEntity(IEntity& entity, bool callback = true);
    void dropEntity(IEntity& entity, bool callback = true);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void update(sf::Time dt);

protected:
    std::vector<IEntity*> entities_;

private:
    virtual void addProperties(IEntity&);
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ISYSTEM_H
