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
 * @brief Implementation for ISystem
*/

#include "CompuBrite/SFML/ISystem.h"
#include "CompuBrite/SFML/IEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace CompuBrite::SFML {

void
ISystem::draw(Context &, sf::RenderStates) const
{
}

void
ISystem::update(Context &, sf::Time dt)
{
}

void
ISystem::addEntity(IEntity &entity, bool callback)
{
    Lock lock(mutex_);
    auto found = std::find(entities_.begin(), entities_.end(), &entity);
    if (found == entities_.end()) {
        addProperties(entity);
        entities_.push_back(&entity);
        lock.unlock();
        if (callback) {
            entity.addSystem(*this, false);
        }
    }
}

void
ISystem::dropEntity(IEntity &entity, bool callback)
{
    Lock lock(mutex_);
    auto found = std::find(entities_.begin(), entities_.end(), &entity);
    if (found != entities_.end()) {
        entities_.erase(found);
        lock.unlock();
        if (callback) {
            entity.dropSystem(*this, false);
        }
    }
}

void
ISystem::addProperties(IEntity &)
{
}

} // namespace CompuBrite::SFML
