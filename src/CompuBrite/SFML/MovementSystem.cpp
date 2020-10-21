#include "CompuBrite/SFML/MovementSystem.h"

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
 * @brief Implementation for MovementSystem
*/

#include <SFML/System/Vector2.hpp>

namespace CompuBrite::SFML {

void
MovementSystem::update(sf::Time dt)
{
    for (auto entity: entities_) {
        auto &vel = entity->properties.ref<sf::Vector2f>("velocity");
        auto &rot = entity->properties.ref<float>("rotation");

        auto acc = entity->properties.get<sf::Vector2f>("acceleration");
        auto racc = entity->properties.get<float>("rot_accel");

        auto t = dt.asSeconds();

        vel += acc * t;
        rot += racc * t;

        entity->move(vel * t);
        entity->rotate(rot * t);
    }
}

void
MovementSystem::addProperties(IEntity &entity)
{
    entity.properties.add<sf::Vector2f>("velocity");
    entity.properties.add<float>("rotation");
    entity.properties.add<sf::Vector2f>("acceleration");
    entity.properties.add<float>("rot_accel");
}

} // namespace CompuBrite::SFML
