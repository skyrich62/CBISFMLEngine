#include <sstream>
#include <iomanip>

#include "CompuBrite/SFML/CircleEntity.h"
#include "CompuBrite/SFML/State.h"
#include "CompuBrite/SFML/MovementSystem.h"
#include "CompuBrite/SFML/DrawingSystem.h"
#include "CompuBrite/SFML/CollisionSystem.h"
#include "CompuBrite/SFML/Engine.h"
#include "CompuBrite/SFML/Context.h"
#include "CompuBrite/SFML/RectangleEntity.h"
#include "CompuBrite/SFML/ResourceManager.h"
#include "CompuBrite/SFML/TextEntity.h"

static constexpr auto GRAVITY = 1.625f;

namespace cbisf = CompuBrite::SFML;

enum class Fonts {Vera};
cbisf::ResourceManager<Fonts, sf::Font> fontManager;

class Altitude : public cbisf::ISystem
{
public:
    Altitude() :
        status_("", fontManager.get(Fonts::Vera), 15)
    {
    }

    ~Altitude() = default;

    void update(sf::Time dt) override;

    cbisf::TextEntity &status()             { return status_;  }
    void setMaxAlt(float maxAlt)            { maxAlt_ = maxAlt; }

private:
    void addProperties(cbisf::IEntity &entity);

private:
    cbisf::TextEntity status_;
    float             maxAlt_ = 0.0f;
};

void
Altitude::addProperties(cbisf::IEntity &entity)
{
    entity.properties.add<float>("altitude", 0.0f);
}

void
Altitude::update(sf::Time dt)
{
    for (auto entity: entities_) {
        auto &accel = entity->properties.ref<sf::Vector2f>("acceleration");
        auto &fuel = entity->properties.ref<float>("fuel");
        accel = {0.0f, GRAVITY};
        std::ostringstream os;
        auto alt = maxAlt_ - entity->getPosition().y - 10.0f;
        entity->properties.set<float>("altitude", alt);
        os << std::fixed << "Alt: "
           << std::setw(8) << std::setprecision(2) << alt;
        entity->properties.set<float>("altitude", alt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (fuel > 0.0f) {
                accel -= {0.0f, 15.f };
                fuel -= 1 * dt.asSeconds();
                if (fuel < 0.0f) {
                    fuel = 0.0f;
                }
            }
        }
        os << std::fixed << " Vel: "
           << std::setw(8) << std::setprecision(2)
           << entity->properties.get<sf::Vector2f>("velocity").y
           << "m/s   Fuel: " << std::setw(6) << std::setprecision(2) << fuel;
        status_.setString(os.str());
    }
}

class InstructionState : public cbisf::State
{
public:
    InstructionState() = default;
    ~InstructionState() = default;

    bool draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

bool
InstructionState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static const char *text = R"(
Lunar Landing Simulator.

Use the space bar to apply thrust. When you are out of fuel, you will
not be able to counter act the pull of gravity.

Press <R> to run the simulation.
)";

    static sf::Text instructions(text, fontManager.get(Fonts::Vera), 15);
    target.draw(instructions, states);
    return lastDrawn_;
}

class LandedState : public cbisf::State
{
public:
    using cbisf::State::State;
    ~LandedState() = default;

    void setShip(const cbisf::IEntity &ship)     { ship_ = &ship; }

    bool draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    const cbisf::IEntity *ship_ = nullptr;
};

bool
LandedState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::ostringstream os;
    auto vel = ship_->properties.get<sf::Vector2f>("velocity").y;
    os << std::fixed << std::setprecision(2);
    os << "\n\n\n\n\n";
    os << "Your ship was moving at a velocity of " << vel << "m/s.\n";
    os << "You have ";
    if (vel > 20.0f) {
        os << "crashed!";
    } else if (vel > 10.0f) {
        os << "broken your landing gear.";
    } else {
        os << "landed safely!";
    }
    os << "\n\nPress <ESC> to end the simulator.\n";
    os << "Press <R> to restart.";
    sf::Text text(os.str(), fontManager.get(Fonts::Vera), 15);
    target.draw(text, states);
    return lastDrawn_;
}

class Lander : public cbisf::Engine
{
public:
    Lander() = default;
    ~Lander() = default;

    void init();

public:
    // Size of the world
    static constexpr float   height_ = 800.0f;
    static constexpr float   width_ = 600.0f;
    static constexpr float   maxFuel_ = 15.0f;

private:
    cbisf::CircleEntity      ship_{10.0f, 3};
    cbisf::RectangleEntity   ground_{{width_, 100.0f}};
    cbisf::MovementSystem    ms_;
    cbisf::CollisionSystem   cs_{cbisf::CollisionSystem::AABB};
    cbisf::DrawingSystem     ds_;
    Altitude                 alt_;

    cbisf::State             landingState_;
    InstructionState         iState_;
    LandedState              landedState_{false, true, true};

};

void
Lander::init()
{
    // Initialize the ship.
    ship_.setFillColor(sf::Color::Yellow);
    ship_.setOrigin(5.0f, 5.0f);
    ship_.setPosition(width_ / 2.0f, 10.0f);\
    ship_.properties.add<float>("fuel", maxFuel_);

    ground_.setPosition(0.0f, height_ -10.0f);
    ground_.setFillColor(sf::Color::Red);

    landingState_.addSystem(ms_);
    landingState_.addSystem(cs_);
    landingState_.addSystem(ds_);

    iState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::R}},
    [this](const sf::Event &, cbisf::Context &context) {
        context.stack().pop();
        context.stack().push(landingState_);
    });


    auto &context = addContext("main", sf::seconds(1.0f / 60.0f),
                               sf::VideoMode(Lander::width_, Lander::height_), "Lander");

    context.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Escape}},
    [](const sf::Event &, cbisf::Context &context) {
        context.stack().clear();
    });

    context.addEvent({sf::Event::Closed},
    [](const sf::Event &, cbisf::Context &context) {
        context.stack().clear();
    });

    // Setup the systems.
    ms_.addEntity(ship_);
    ds_.addEntity(ship_);
    ds_.addEntity(ground_);
    cs_.addEntity(ship_);
    cs_.addEntity(ground_);
    alt_.addEntity(ship_);
    ds_.addEntity(alt_.status());

    alt_.setMaxAlt(height_ - 10.0f);

    // Setup the landing state.
    landingState_.addSystem(ms_);
    landingState_.addSystem(cs_);
    landingState_.addSystem(alt_);
    landingState_.addSystem(ds_);

    // Setup collision system
    cs_.addHandler<cbisf::CircleEntity, cbisf::RectangleEntity>(
    [this] (cbisf::CircleEntity &ship, cbisf::RectangleEntity &ground, const sf::FloatRect&) {
        if (ship.properties.get<float>("altitude") <= 0.0f) {
            this->getContext("main").stack().push(landedState_);
        }
    });

    // Push the instruction state.
    context.stack().push(iState_);

    // Setup the landed state.
    landedState_.setShip(ship_);
    landedState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::R}},
    [this](const sf::Event &, cbisf::Context &context) {
        context.stack().clear();
        context.stack().push(landingState_);
        ship_.setPosition({width_ / 2.0f, 10.0f});
        ship_.properties.set<float>("fuel", maxFuel_);
        ship_.properties.set<float>("altitude", height_);
        ship_.properties.set<sf::Vector2f>("velocity", {0.f, 0.f});
        ship_.properties.set<sf::Vector2f>("acceleration", {0.f, 0.f});
    });

}

int main()
{
    fontManager.load(Fonts::Vera, "C:/Users/rnewman/AppData/Local/Microsoft/Windows/Fonts/VeraSe.ttf");
    Lander lander;

    lander.init();

    lander.run();
    return 0;
}
