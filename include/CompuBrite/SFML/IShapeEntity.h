#ifndef COMPUBRITE_SFML_ISHAPEENTITY_H
#define COMPUBRITE_SFML_ISHAPEENTITY_H

#include <CompuBrite/SFML/IEntity.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace CompuBrite
{
namespace SFML
{

class IShapeEntity : public CompuBrite::SFML::IEntity
{
public:
    virtual ~IShapeEntity() = default;

    void setTexture(const sf::Texture *texture, bool resetRect = false);
    void setTextureRect(const sf::IntRect &rectangle);
    void setFillColor(const sf::Color &color);
    void setOutlineColor(const sf::Color &color);
    void setOutlineThickness(float thickness);

    const sf::Texture *getTexture() const;
    const sf::IntRect &getTextureRect() const;
    const sf::Color &getFillColor() const;
    const sf::Color &getOutlineColor() const;
    float getOutlineThickness() const;

    std::size_t getPointCount() const;
    sf::Vector2f getPoint(std::size_t index) const;

    sf::FloatRect getLocalBounds() const override;

protected:
    explicit IShapeEntity(sf::Shape &shape);

private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Shape *shape_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_ISHAPEENTITY_H
