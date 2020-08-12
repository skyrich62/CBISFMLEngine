#include "CompuBrite/SFML/DrawingSystem.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite {
namespace SFML {

void
DrawingSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto entity : entities_) {
        target.draw(*entity, states);
    }
}

} // namespace SFML
} // namespace CompuBrite
