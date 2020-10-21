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
 * @brief Implementation for DrawingSystem
*/

#include "CompuBrite/SFML/DrawingSystem.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <map>

namespace CompuBrite::SFML {

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

} // namespace CompuBrite::SFML
