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
 * @brief Interface for IProperty
*/

#ifndef COMPUBRITE_SFML_IPROPERTY_H
#define COMPUBRITE_SFML_IPROPERTY_H

#include <string>
#include <typeinfo>

namespace CompuBrite
{
namespace SFML
{

/// IProperty is a base-class for TProperty.  A property is a name, type,
/// value "tuple".
/// @see PropertyManager
/// @see TProperty
/// @see MovementSystem
class IProperty
{
public:
    /// Construct the property with the given name and type.
    IProperty(const std::string &name, const std::type_info &type) :
    name_(name),
    type_(type)
    { }

    virtual ~IProperty() = default;

    /// @return The name of this IProperty.
    const std::string &name() const         { return name_; }

    /// @return The type of this IProperty.
    const std::type_info *type() const      { return &type_; }

private:
    std::string name_;
    const std::type_info &type_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_IPROPERTY_H
