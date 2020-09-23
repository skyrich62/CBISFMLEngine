#ifndef COMPUBRITE_SFML_STATESTACK_H
#define COMPUBRITE_SFML_STATESTACK_H

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace CompuBrite
{
namespace SFML
{

class State;
class ISystem;

class StateStack : public sf::Drawable
{
public:
    StateStack() = default;
    ~StateStack() = default;

    void push(State &state);
    State *top() const;
    State *pop();
    void clear();
    bool empty()                            { return states_.empty(); }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(sf::Time dt);

private:
    std::vector<State *> states_;
};

} // namespace SFML
} // namespace CompuBrite

#endif // COMPUBRITE_SFML_STATESTACK_H
