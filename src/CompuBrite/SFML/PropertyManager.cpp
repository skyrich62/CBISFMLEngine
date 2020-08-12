#include "CompuBrite/SFML/PropertyManager.h"

namespace CompuBrite {
namespace SFML {

IProperty *PropertyManager::ifind(const std::string &name) const
{
    auto found = properties_.find(name);
    if (found == properties_.end()) {
        return nullptr;
    }
    return found->second.get();
}

} // namespace SFML
} // namespace CompuBrite
