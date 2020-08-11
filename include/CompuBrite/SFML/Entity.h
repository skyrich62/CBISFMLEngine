#ifndef COMPUBRITE_SFML_ENTITY_H
#define COMPUBRITE_SFML_ENTITY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <map>
#include <functional>

namespace CompuBrite {
namespace SFML {

class Entity : public sf::Drawable, public sf::Transformable
{
public:

    Entity() : Entity(0) { }
    explicit Entity(int zOrder) { }
    virtual ~Entity();

    virtual sf::FloatRect getLocalBounds() const;
    bool addChild(Entity &child, int zOrder = 0);

public:
    void update(sf::Time dt);

private:
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void draw (sf::RenderTarget &target, sf::RenderStates states) const final;
    void updateChildren(sf::Time dt);

    virtual void updateThis(sf::Time dt);
    virtual void drawThis(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    using zLevel = std::vector<Entity*>;
    using Children = std::map<int, zLevel>;

    Children children_;
    Entity *parent_{nullptr};
    int zOrder_{0};

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENTITY_H
