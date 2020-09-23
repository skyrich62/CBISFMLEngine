#ifndef COMPUBRITE_SFML_ENTITY_H
#define COMPUBRITE_SFML_ENTITY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <CompuBrite/SFML/PropertyManager.h>


#include <vector>
#include <functional>

namespace CompuBrite {
namespace SFML {

class ISystem;

class IEntity : public sf::Drawable, public sf::Transformable
{
public:

    IEntity() = default;
    explicit IEntity(int zOrder) : zOrder_(zOrder) { }
    virtual ~IEntity();

    virtual sf::FloatRect getLocalBounds() const;
    bool addChild(IEntity &child, int zOrder);
    bool addChild(IEntity &child);
    void update(sf::Time dt);
    void addSystem(ISystem &system, bool callback = true);
    void dropSystem(ISystem &system, bool callback = true);
    sf::Vector2f getGlobalPosition() const;
    sf::FloatRect getGlobalBounds() const;
    sf::Transform getGlobalTransform() const;
    int zOrder() const                            { return zOrder_; }
    void zOrder(int order)                        { zOrder_ = order; }

private:
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void draw (sf::RenderTarget &target, sf::RenderStates states) const final;
    void updateChildren(sf::Time dt);


    virtual void updateThis(sf::Time dt);
    virtual void drawThis(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    using Children = std::vector<IEntity*>;
    using Systems = std::vector<ISystem*>;

    Children children_;
    Systems systems_;
    IEntity *parent_{nullptr};
    int zOrder_{0};

public:
    PropertyManager properties;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENTITY_H
