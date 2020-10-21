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
 * @brief Interface for PropertyManager
*/

#ifndef COMPUBRITE_SFML_PROPERTYMANAGER_H
#define COMPUBRITE_SFML_PROPERTYMANAGER_H

#include <CompuBrite/SFML/TProperty.h>
#include <CompuBrite/CheckPoint.h>

#include <map>
#include <string>
#include <memory>

namespace CompuBrite::SFML {

/// PropertyManager is the means of extending a class by containing
/// name / type / value tuples.
/// @see TProperty
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

    /// Add a new property to his manager.
    /// @tparam Type The type of this new property.
    /// @param name The name of the property.
    /// @param value the optional given value for the new property.  If a value
    /// is not provided, it will be default constructed.
    /// If a property with the same name exists, this call will be silently
    /// ignored.
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

    /// Set the value of the given property to the given value.
    /// If no property exists with the given name, then this call will be
    /// silently ignored.
    /// @tparam The type of the property.
    /// @param name The name of the property.
    /// @param value The new value for the property.
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

    /// Get the value for the given property.  If no property by the given
    /// name exists, or if a property does exist by that name, but with a
    /// different type, then a default constructed value will be returned.
    /// @tparam Type The type of the property.
    /// @param name The name of the property.
    /// @return The value of the given property if it exists, else a default
    /// constructed object of the given type will be returned.
    template <typename Type>
    Type get(const std::string &name) const
    {
        auto p = find<Type>(name);
        if (!p) {
            return Type();
        }
        return p->get();
    }

    /// Return a reference to the given property.
    /// @tparam Type The type of the property.
    /// @param name The name of the requested property.
    /// @return A reference to the requtest property if it exists.  It does
    /// not exist, or if a property does exists with the given name, but
    /// with a different type, then a reference to a nullptr will be
    /// returned.
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

    /// @override
    /// Return a constant reference to the given property, or a constant
    /// reference to a nullptr if one cannot be found.
    template <typename Type>
    const Type &ref(const std::string &name) const
    {
        return cref<Type>(name);
    }

    /// Return a constant reference to the given property, or a constant
    /// reference to a nullptr if one cannot be found.
    /// @see ref()
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

    /// Find a property with the given type and name.
    /// @tparam Type The requested type.
    /// @param name The name of the requested property.
    /// @return A pointer to the TProperty object if it exists, a nullptr
    /// otherwise.
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
    /// Find a property with the given name.
    /// @return A pointer to an IProperty object with the given name if it
    /// exists, a nullptr otherwise.
    IProperty *ifind(const std::string &name) const;

    using Properties = std::map<std::string, Ptr>;
    Properties properties_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_PROPERTYMANAGER_H
