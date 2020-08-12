#ifndef COMPUBRITE_SFML_DRAWINGSYSTEM_H
#define COMPUBRITE_SFML_DRAWINGSYSTEM_H

#include <CompuBrite/SFML/ISystem.h>

namespace CompuBrite {
namespace SFML {

class DrawingSystem : public CompuBrite::SFML::ISystem
{
    public:
        DrawingSystem() = default;
        virtual ~DrawingSystem() = default;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_DRAWINGSYSTEM_H
