#ifndef COMPUBRITE_SFML_PROPERTYMANAGER_H
#define COMPUBRITE_SFML_PROPERTYMANAGER_H

#include <CompuBrite/SFML/TProperty.h>
#include <CompuBrite/CheckPoint.h>

#include <map>
#include <string>
#include <memory>

namespace CompuBrite
{
namespace SFML
{

class PropertyManager
{
public:
    using Ptr = std::unique_ptr<CompuBrite::SFML::IProperty>;

    PropertyManager() = default;
    ~PropertyManager() = default;
    PropertyManager(const PropertyManager& other) = delete;
    PropertyManager(PropertyManager &&other) = default;

    PropertyManager& operator=(const PropertyManager& other) = delete;
    PropertyManager& operator=(PropertyManager &&other) = default;

    template <typename Type>
    void add(const std::string &name, Type value = Type())
    {
        if (ifind(name)) {
            // If the property already exists, silently ignore this request.
            return;
        }
        auto ptr = std::make_unique<TProperty<Type>>(name, value);
        properties_[name] = std::move(ptr);
    }

    template <typename Type>
    void set(const std::string &name, Type value)
    {
        auto p = find<Type>(name);
        if (!p) {
            // silently ignore.
            return;
        }
        auto prop = dynamic_cast<TProperty<Type>*>(p);
        prop->set(value);
    }

    template <typename Type>
    Type get(const std::string &name) const
    {
        auto p = find<Type>(name);
        if (!p) {
            return Type();
        }
        return p->get();
    }

    template <typename Type>
    Type &ref(const std::string &name)
    {
        auto p = find<Type>(name);
        if (!p) {
            Type *r = nullptr;
            CompuBrite::CheckPoint::hit(CBI_HERE, "Property not found: ", name);
            return *r;
        }
        return p->ref();
    }

    template <typename Type>
    const Type &ref(const std::string &name) const
    {
        return cref<Type>(name);
    }

    template <typename Type>
    const Type &cref(const std::string &name) const
    {
        auto p = find<Type>(name);
        if (!p) {
            const Type * r = nullptr;
            CompuBrite::CheckPoint::hit(CBI_HERE, "Property not found: ", name);
            return *r;
        }
        return p->cref();
    }

    template <typename Type>
    TProperty<Type> *find(const std::string &name) const
    {
        auto p = ifind(name);
        if (p) {
            if (&typeid(Type) == p->type()) {
                return dynamic_cast<TProperty<Type>*>(p);
            }
        }
        return nullptr;
    }
private:
    IProperty *ifind(const std::string &name) const;
    using Properties = std::map<std::string, Ptr>;
    Properties properties_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_PROPERTYMANAGER_H
