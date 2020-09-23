#include "CompuBrite/SFML/DrawingSystem.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <map>

namespace CompuBrite {
namespace SFML {

DrawingSystem::DrawingSystem(bool boundingBoxes) :
    ISystem(),
    boundingBoxes_(boundingBoxes)
{
}

void
DrawingSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::multimap<int, IEntity*> drawings;
    for (auto entity : entities_) {
        drawings.insert(std::pair<int, IEntity*>(entity->zOrder(), entity));
    }
    for (auto entity: drawings) {
        target.draw(*entity.second, states);
        if (boundingBoxes_) {
            auto bounds = entity.second->getGlobalBounds();
            sf::Vector2f size(bounds.width, bounds.height);
            sf::RectangleShape box(size);
            box.setPosition(bounds.left, bounds.top);
            box.setOutlineColor(sf::Color::Green);
            box.setFillColor(sf::Color::Transparent);
            box.setOutlineThickness(2.0f);
            target.draw(box);
        }
    }
}

} // namespace SFML
} // namespace CompuBrite
