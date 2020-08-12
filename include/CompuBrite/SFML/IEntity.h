#ifndef COMPUBRITE_SFML_ENTITY_H
#define COMPUBRITE_SFML_ENTITY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <CompuBrite/SFML/PropertyManager.h>


#include <vector>
#include <map>
#include <functional>

namespace CompuBrite {
namespace SFML {

class ISystem;

class IEntity : public sf::Drawable, public sf::Transformable
{
public:

    IEntity() : IEntity(0) { }
    explicit IEntity(int zOrder) { }
    virtual ~IEntity();

    virtual sf::FloatRect getLocalBounds() const;
    bool addChild(IEntity &child, int zOrder = 0);
    void update(sf::Time dt);
    void addSystem(ISystem &system, bool callback = true);
    void dropSystem(ISystem &system, bool callback = true);


private:
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void draw (sf::RenderTarget &target, sf::RenderStates states) const final;
    void updateChildren(sf::Time dt);

    virtual void updateThis(sf::Time dt);
    virtual void drawThis(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    using zLevel = std::vector<IEntity*>;
    using Children = std::map<int, zLevel>;

    Children children_;
    IEntity *parent_{nullptr};
    int zOrder_{0};
    std::vector<ISystem*> systems_;

public:
    PropertyManager properties;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENTITY_H
