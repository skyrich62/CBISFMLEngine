#include "CompuBrite/SFML/SpriteEntity.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite {
namespace SFML {

SpriteEntity::SpriteEntity(int zOrder, const sf::Texture &texture) :
    IEntity(zOrder),
    sprite_(texture)
{
}

SpriteEntity::SpriteEntity(int zOrder, const sf::Texture &texture, const sf::IntRect &rect) :
    IEntity(zOrder),
    sprite_(texture, rect)
{
}

SpriteEntity::SpriteEntity(const sf::Texture &texture) :
    SpriteEntity(0, texture)
{
}

SpriteEntity::SpriteEntity(const sf::Texture &texture, const sf::IntRect &rect) :
    SpriteEntity(0, texture, rect)
{
}

void
SpriteEntity::setTexture(const sf::Texture &texture, bool resetRect)
{
    sprite_.setTexture(texture, resetRect);
}

void
SpriteEntity::setTextureRect(const sf::IntRect &rectangle)
{
    sprite_.setTextureRect(rectangle);
}

const sf::Texture *
SpriteEntity::getTexture() const
{
    return sprite_.getTexture();
}

const sf::IntRect &
SpriteEntity::getTextureRect() const
{
    return sprite_.getTextureRect();
}

const sf::Color &
SpriteEntity::getColor() const
{
    sprite_.getColor();
}

sf::FloatRect
SpriteEntity::getLocalBounds() const
{
    return sprite_.getLocalBounds();
}

void
SpriteEntity::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}


} // namespace SFML
} // namespace CompuBrite
