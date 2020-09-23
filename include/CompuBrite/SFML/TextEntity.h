#ifndef COMPUBRITE_SFML_TEXTENTITY_H
#define COMPUBRITE_SFML_TEXTENTITY_H

#include <CompuBrite/SFML/IEntity.h>

#include <SFML/Graphics/Text.hpp>

namespace CompuBrite
{
namespace SFML
{

class TextEntity : public CompuBrite::SFML::IEntity
{
public:
    TextEntity() = default;
    TextEntity(const sf::String &string, const sf::Font &font, unsigned int size);
    virtual ~TextEntity() = default;

    sf::FloatRect getLocalBounds() const override;

    // Delegation
    void setString(const sf::String &string)     { text_.setString(string); }
    void setFont(const sf::Font &font)           { text_.setFont(font); }
    void setCharacterSize(unsigned int size)     { text_.setCharacterSize(size); }
    void setLineSpacing(float spacing)           { text_.setLineSpacing(spacing); }
    void setLetterSpacing(float spacing)         { text_.setLetterSpacing(spacing); }
    void setStyle(sf::Uint32 style)              { text_.setStyle(style); }
    //void setColor(const sf::Color &color)        { text_.setColor(color); }
    void setFillColor(const sf::Color &color)    { text_.setFillColor(color); }
    void setOutlineColor(const sf::Color &color) { text_.setOutlineColor(color); }
    void setOutlineThickness(float thickness)    { text_.setOutlineThickness(thickness); }
    const sf::String getString() const           { return text_.getString(); }
    const sf::Font *getFont() const              { return text_.getFont(); }
    unsigned int getCharacterSize() const        { return text_.getCharacterSize(); }
    float getLetterSpacing() const               { return text_.getLetterSpacing(); }
    float getLineSpacing() const                 { return text_.getLineSpacing(); }
    sf::Uint32 getStyle() const                  { return text_.getStyle(); }
    //const sf::Color &getColor() const            { return text_.getColor(); }
    const sf::Color &getOutlineColor() const     { return text_.getOutlineColor(); }
    const sf::Color &getFillColor() const        { return text_.getFillColor(); }
    float getOutlineThickness() const            { return text_.getOutlineThickness(); }

    sf::Vector2f findCharacterPos(std::size_t index) const
                                                 { return text_.findCharacterPos(index); }

private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Text text_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_TEXTENTITY_H
