#ifndef COMPUBRITE_SMFL_RESOURCEMANAGER_H
#define COMPUBRITE_SMFL_RESOURCEMANAGER_H

#include <map>
#include <memory>
#include <utility>

#include "CompuBrite/CheckPoint.h"

namespace CompuBrite
{
namespace SFML
{

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

    const Resource& get(const ID &id) const
    {
        auto r = find(id);
        CompuBrite::CheckPoint::expect(CBI_HERE, r, "Resource not found!");
        return *r;
    }

    Resource& get(const ID &id)
    {
        auto r = find(id);
        CompuBrite::CheckPoint::expect(CBI_HERE, r, "Resource not found!");
        return *r;
    }

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

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SMFL_RESOURCEMANAGER_H
