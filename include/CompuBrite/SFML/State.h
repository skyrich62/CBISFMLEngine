#ifndef COMPUBRITE_SFML_STATE_H
#define COMPUBRITE_SFML_STATE_H

#include <vector>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>


namespace CompuBrite
{
namespace SFML
{
class ISystem;

class State
{
public:
    State() = default;
    State(bool lastDrawn, bool lastUpdated);
    virtual ~State() = default;

    virtual bool draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual bool update(sf::Time dt);

    void addSystem(ISystem &system);
    void dropSystem(ISystem &system);

    void lastDrawn(bool b)                  { lastDrawn_ = b; }
    void lastUpdated(bool b)                { lastUpdated_ = b; }

private:
    std::vector<ISystem*> systems_;
    bool                  lastDrawn_ = false;
    bool                  lastUpdated_ = false;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_STATE_H
