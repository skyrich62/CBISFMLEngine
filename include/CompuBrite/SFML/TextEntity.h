/**
 * The MIT License (MIT)
 *
 * @copyright
 * Copyright (c) 2020 Rich Newman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file
 * @brief Interface for TextEntity
*/

#ifndef COMPUBRITE_SFML_TEXTENTITY_H
#define COMPUBRITE_SFML_TEXTENTITY_H

#include <CompuBrite/SFML/IEntity.h>

#include <SFML/Graphics/Text.hpp>

namespace CompuBrite
{
namespace SFML
{

/// Encapsulates a sf::Text object and brings it under the IEntity framework.
class TextEntity : public CompuBrite::SFML::IEntity
{
public:
    TextEntity() = default;

    /// Construct sf::Text object with the given parameters.
    /// Deletegates to sf::Text::Text()
    /// @param string The string to use for the sf::Text object.
    /// @param font A reference to the requested sf::Font.
    /// @param size The size of the text in points.
    TextEntity(const sf::String &string, const sf::Font &font, unsigned int size);
    virtual ~TextEntity() = default;

    /// @return the Axis-Aligned Boundary Box for this Text object.
    sf::FloatRect getLocalBounds() const override;

    /// @name Delegation
    /// @{
    void setString(const sf::String &string)     { text_.setString(string); }
    void setFont(const sf::Font &font)           { text_.setFont(font); }
    void setCharacterSize(unsigned int size)     { text_.setCharacterSize(size); }
    void setLineSpacing(float spacing)           { text_.setLineSpacing(spacing); }
    void setLetterSpacing(float spacing)         { text_.setLetterSpacing(spacing); }
    void setStyle(sf::Uint32 style)              { text_.setStyle(style); }
    void setFillColor(const sf::Color &color)    { text_.setFillColor(color); }
    void setOutlineColor(const sf::Color &color) { text_.setOutlineColor(color); }
    void setOutlineThickness(float thickness)    { text_.setOutlineThickness(thickness); }
    const sf::String getString() const           { return text_.getString(); }
    const sf::Font *getFont() const              { return text_.getFont(); }
    unsigned int getCharacterSize() const        { return text_.getCharacterSize(); }
    float getLetterSpacing() const               { return text_.getLetterSpacing(); }
    float getLineSpacing() const                 { return text_.getLineSpacing(); }
    sf::Uint32 getStyle() const                  { return text_.getStyle(); }
    const sf::Color &getOutlineColor() const     { return text_.getOutlineColor(); }
    const sf::Color &getFillColor() const        { return text_.getFillColor(); }
    float getOutlineThickness() const            { return text_.getOutlineThickness(); }

    sf::Vector2f findCharacterPos(std::size_t index) const
                                                 { return text_.findCharacterPos(index); }

    /// @}
private:
    /// Draw this sf::Text object
    /// Delegates to sf::Text::draw()
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Text text_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_TEXTENTITY_H
