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
 * @brief Interface for TProperty
*/

#ifndef COMPUBRITE_SFML_TPROPERTY_H_INCLUDED
#define COMPUBRITE_SFML_TPROPERTY_H_INCLUDED

#include "CompuBrite/SFML/IProperty.h"


namespace CompuBrite::SFML {

/// A TProperty is a name / type / value tuple.  It is used to provide arbitrary
/// properties into IEntity objects for use by the ISystem mechanism.
/// @see PropertyManager
/// @see MovementSystem
/// @tparam Type  The type for this property.
template <typename Type>
class TProperty : public IProperty
{
public:
    /// Construct a TProperty of this type with the given name and value.
    /// @param name The name for this property.
    /// @param value The value for this property.  If not provided, a default
    /// value will be used.
    TProperty(const std::string name, Type value = Type()) :
        IProperty(name, typeid(Type)),
        value_(value)
    {
    }

    /// Set the value for this property.
    void set(const Type& value)             { value_ = value; }

    /// @override
    void set(Type&& value)                  { value_ = std::move(value); }

    /// @return The value associated with this property.
    Type get(void) const                    { return value_; }

    /// @return a reference to the value for this property.
    Type &ref(void)                         { return value_; }

    /// @override
    /// @return a constant reference.
    const Type &ref(void)  const            { return value_; }

    /// @return a constant reference to the value of this property.
    const Type &cref(void) const            { return value_; }

private:
    Type                  value_;
};

} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_TPROPERTY_H_INCLUDED
