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
 * @brief Interface for ResourceManager
*/

#ifndef COMPUBRITE_SMFL_RESOURCEMANAGER_H
#define COMPUBRITE_SMFL_RESOURCEMANAGER_H

#include <map>
#include <memory>
#include <utility>

#include "CompuBrite/CheckPoint.h"

namespace CompuBrite::SFML {

/// Manages arbitrary resources by assigning arbitrary identifiers to them.
/// @tparam ID The type of the identifier to use.
/// @tparam Resource The type of the resource to manage.
template<typename ID, typename Resource>
class ResourceManager
{
public:
    using Self = ResourceManager<ID, Resource>;
    using Ptr = std::unique_ptr<Resource>;

    ResourceManager() = default;
    ResourceManager(const Self&) = delete;
    ResourceManager(Self&&) = default;
    ~ResourceManager() = default;

    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = default;

    /// Get a const reference to a resource.
    /// @param id The identifier associated to the requested resource.
    /// @return A const reference to the requested resource, or a const
    /// reference to nullptr.
    const Resource& get(const ID &id) const
    {
        auto r = find(id);
        CompuBrite::CheckPoint::expect(CBI_HERE, r, "Resource not found!");
        return *r;
    }

    /// Get a reference to a resource
    /// @override.
    /// This will return a non-const reference.
    Resource& get(const ID &id)
    {
        auto r = find(id);
        CompuBrite::CheckPoint::expect(CBI_HERE, r, "Resource not found!");
        return *r;
    }

    /// Load the requested resource from the given file. This will add the
    /// resource to this manager, load the resource from the file, and associate
    /// the given identifier to the resource.
    ///
    /// Note: calls sf::Resource::loadFromFile().
    /// @param id The identifier to associate with this resource.
    /// @param filename The filename to load from.
    /// return true if the resource was found and loaded, false otherwise.
    bool load(const ID &id, const std::string &filename)
    {
        if (find(id)) {
            CompuBrite::CheckPoint::hit(CBI_HERE, "Resource already loaded.");
            return false;
        }
        auto res = std::make_unique<Resource>();
        if (!res->loadFromFile(filename)) {
            throw std::runtime_error("failed to load file: " + filename);
        }
        resources_[id] = std::move(res);
        return true;
    }

private:
    /// Fine the given resource if it exists.
    /// @param id The identifier to look for.
    /// @return The resource associated to id if it exists, else nullptr.
    Resource* find(const ID &id)
    {
        auto found = resources_.find(id);
        if (found == resources_.end()) {
            return nullptr;
        }
        return found->second.get();
    }

private:
    std::map<ID, Ptr> resources_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SMFL_RESOURCEMANAGER_H
