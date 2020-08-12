#ifndef COMPUBRITE_SFML_IPROPERTY_H
#define COMPUBRITE_SFML_IPROPERTY_H

#include <string>
#include <typeinfo>

namespace CompuBrite
{
namespace SFML
{

class IProperty
{
public:
    IProperty(const std::string &name, const std::type_info &type) :
    name_(name),
    type_(type)
    { }

    virtual ~IProperty() = default;

    const std::string &name() const         { return name_; }
    const std::type_info *type() const      { return &type_; }

private:
    std::string name_;
    const std::type_info &type_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_IPROPERTY_H
