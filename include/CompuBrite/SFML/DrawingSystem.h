#ifndef COMPUBRITE_SFML_DRAWINGSYSTEM_H
#define COMPUBRITE_SFML_DRAWINGSYSTEM_H

#include <CompuBrite/SFML/ISystem.h>

namespace CompuBrite {
namespace SFML {

class DrawingSystem : public CompuBrite::SFML::ISystem
{
    public:
        explicit DrawingSystem(bool boundingBoxes = false);
        virtual ~DrawingSystem() = default;

    protected:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        bool boundingBoxes_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_DRAWINGSYSTEM_H
