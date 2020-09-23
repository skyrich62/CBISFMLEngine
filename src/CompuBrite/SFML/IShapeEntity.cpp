#include "CompuBrite/SFML/IShapeEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite {
namespace SFML {

IShapeEntity::IShapeEntity(sf::Shape &shape) :
    shape_(&shape)
{
    //ctor
}

void
IShapeEntity::setTexture(const sf::Texture *texture, bool resetRect)
{
    shape_->setTexture(texture, resetRect);
}

void
IShapeEntity::setTextureRect(const sf::IntRect &rectangle)
{
    shape_->setTextureRect(rectangle);
}

void
IShapeEntity::setFillColor(const sf::Color &color)
{
    shape_ ->setFillColor(color);
}

void
IShapeEntity::setOutlineColor(const sf::Color &color)
{
    shape_ ->setOutlineColor(color);
}

void
IShapeEntity::setOutlineThickness(float thickness)
{
    shape_->setOutlineThickness(thickness);
}

const sf::Texture *
IShapeEntity::getTexture() const
{
    return shape_->getTexture();
}

const sf::IntRect &
IShapeEntity::getTextureRect() const
{
    return shape_->getTextureRect();
}

const sf::Color &
IShapeEntity::getFillColor() const
{
    return shape_->getFillColor();
}

const sf::Color &
IShapeEntity::getOutlineColor() const
{
    return shape_->getOutlineColor();
}

float
IShapeEntity::getOutlineThickness() const
{
    return shape_->getOutlineThickness();
}

std::size_t
IShapeEntity::getPointCount() const
{
    return shape_->getPointCount();
}

sf::Vector2f
IShapeEntity::getPoint(std::size_t index) const
{
    return shape_->getPoint(index);
}

void
IShapeEntity::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*shape_, states);
}

sf::FloatRect
IShapeEntity::getLocalBounds() const
{
    return shape_->getLocalBounds();
}


} // namespace SFML
} // namespace CompuBrite
