#ifndef COMPUBRITE_SFML_SPRITEENTITY_H
#define COMPUBRITE_SFML_SPRITEENTITY_H

#include <CompuBrite/SFML/IEntity.h>
#include <SFML/Graphics/Sprite.hpp>

namespace CompuBrite
{
namespace SFML
{

class SpriteEntity : public CompuBrite::SFML::IEntity
{
public:
    SpriteEntity() = default;
    explicit SpriteEntity(const sf::Texture &texture);
    SpriteEntity(const sf::Texture &texture, const sf::IntRect &rectangle);
    SpriteEntity(int zOrder, const sf::Texture &texture);
    SpriteEntity(int zOrder, const sf::Texture &texture, const sf::IntRect &rectangle);

    virtual ~SpriteEntity() = default;

    sf::FloatRect getLocalBounds() const override;

    void setTexture(const sf::Texture &texture, bool resetRect = false);
    void setTextureRect(const sf::IntRect &rectangle);
    void setColor(const sf::Color &color);

    const sf::Texture *getTexture() const;
    const sf::IntRect &getTextureRect() const;
    const sf::Color &getColor() const;
private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Sprite sprite_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_SPRITEENTITY_H
