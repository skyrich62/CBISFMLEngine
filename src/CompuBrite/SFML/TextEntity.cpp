#include "CompuBrite/SFML/TextEntity.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite {
namespace SFML {

TextEntity::TextEntity(const sf::String &string, const sf::Font &font, unsigned int size) :
    text_(string, font, size)
{
}

void
TextEntity::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(text_, states);
}

sf::FloatRect
TextEntity::getLocalBounds() const
{
    return text_.getLocalBounds();
}

} // namespace SFML
} // namespace CompuBrite
