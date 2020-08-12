#ifndef COMPUBRITE_SFML_TPROPERTY_H_INCLUDED
#define COMPUBRITE_SFML_TPROPERTY_H_INCLUDED

#include "CompuBrite/SFML/IProperty.h"


namespace CompuBrite {
namespace SFML {

template <typename Type>
class TProperty : public IProperty
{
public:
    TProperty(const std::string name, Type value = Type()) :
        IProperty(name, typeid(Type)),
        value_(value)
    {
    }

    void set(const Type& value)             { value_ = value; }
    void set(Type&& value)                  { value_ = std::move(value); }

    Type get(void) const                    { return value_; }
    Type &ref(void)                         { return value_; }
    const Type &ref(void)  const            { return value_; }
    const Type &cref(void) const            { return value_; }

private:
    Type                  value_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_TPROPERTY_H_INCLUDED
