#ifndef COMPUBRITE_SFML_ENTITY_H
#define COMPUBRITE_SFML_ENTITY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>
#include <functional>

namespace CompuBrite {
namespace SFML {

struct EntityImpl
{
    using DrawThis = std::function<void(sf::RenderTarget&, sf::RenderStates)>;
    using LocalBounds = std::function<sf::FloatRect()>;

    DrawThis drawThis_ = [](sf::RenderTarget&, sf::RenderStates) { };
    LocalBounds getLocalBounds_ = []() { return sf::FloatRect(); };
};

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity(EntityImpl impl = EntityImpl());
    virtual ~Entity();
    sf::FloatRect getLocalBounds() const { return impl_.getLocalBounds_(); };

private:
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    void draw (sf::RenderTarget &target, sf::RenderStates states) const final;

private:
    Entity *parent_ = nullptr;
    std::vector<Entity*> children_;
    EntityImpl impl_;

};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ENTITY_H
