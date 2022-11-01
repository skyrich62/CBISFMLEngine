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
auto gravity_modifier = 0.0f;

namespace cbisf = CompuBrite::SFML;

enum class Fonts {Vera};
cbisf::ResourceManager<Fonts, sf::Font> fontManager;

class PausedState : public cbisf::State
{
public:
    PausedState();
    ~PausedState() = default;

    bool update(sf::Time dt, cbisf::Context &) override;
    bool draw(cbisf::Context &target, sf::RenderStates states) const override;

private:
    cbisf::RectangleEntity _background;
    cbisf::TextEntity      _paused{"Paused - Press 'P' to continue",
                                    fontManager.get(Fonts::Vera), 25};

};

PausedState::PausedState() :
    State(false, true, true)
{
    _background.setFillColor({0, 0, 0, 127});
    sf::Vector2f size(600.0f, 800.0f);
    _background.setSize(size);
    _background.setOrigin(size / 2.0f);
    _background.setPosition(size / 2.0f);
    _paused.setFillColor(sf::Color::Red);
    auto bounds = _paused.getLocalBounds();
    sf::Vector2f sizeText(bounds.width, bounds.height);
    _paused.setOrigin(sizeText / 2.0f);
    _paused.setPosition(size / 2.0f);
    _paused.startBlinking(0.5f);
}

bool
PausedState::update(sf::Time dt, cbisf::Context &)
{
    namespace cbi = CompuBrite;

    _paused.update(dt);
    return true;
}

bool
PausedState::draw(cbisf::Context &target, sf::RenderStates states) const
{
    auto &win = target.window();
    win.draw(_background);
    win.draw(_paused);
    return false;
}

class Altitude : public cbisf::ISystem
{
public:
    Altitude() :
        altitude_("", fontManager.get(Fonts::Vera), 15),
        velocity_("", fontManager.get(Fonts::Vera), 15),
        gravity_("", fontManager.get(Fonts::Vera), 15),
        fuel_("", fontManager.get(Fonts::Vera), 15)
    {
        altitude_.setPosition(180.0f, 0.0f);
        velocity_.setPosition(300.0f, 0.0f);
        fuel_.setPosition(425.0f, 0.0f);
        gravity_.setPosition(0.0f, 0.0f);
    }

    ~Altitude() = default;

    void update(cbisf::Context &context, sf::Time dt) override;

    void setMaxAlt(float maxAlt)            { maxAlt_ = maxAlt; }
    void acceptSystem(cbisf::ISystem &system);

private:
    void addProperties(cbisf::IEntity &entity);

private:
    cbisf::TextEntity altitude_;
    cbisf::TextEntity velocity_;
    cbisf::TextEntity gravity_;
    cbisf::TextEntity fuel_;
    float             maxAlt_ = 0.0f;
};

void
Altitude::addProperties(cbisf::IEntity &entity)
{
    entity.properties.add<float>("altitude", 0.0f);
}

void
Altitude::update(cbisf::Context &context, sf::Time dt)
{
    for (auto entity: entities_) {
        auto &accel = entity->properties.ref<sf::Vector2f>("acceleration");
        auto &fuel = entity->properties.ref<float>("fuel");
        accel = {0.0f, GRAVITY + gravity_modifier};
        std::ostringstream os;
        auto alt = maxAlt_ - entity->getPosition().y - 10.0f;
        entity->properties.set<float>("altitude", alt);
        os << std::fixed << "Alt: "
           << std::setw(8) << std::setprecision(2) << alt;
        altitude_.setString(os.str());
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
        os.str("");
        os << std::fixed << "Vel: "
           << std::setw(8) << std::setprecision(2)
           << entity->properties.get<sf::Vector2f>("velocity").y
           << "m/s";
        velocity_.setString(os.str());
        os.str("");
        os << "Fuel: " << std::setw(6) << std::setprecision(2) << fuel;
        fuel_.setString(os.str());
        if (fuel > 10.0f) {
            fuel_.setFillColor(sf::Color::White);
        } else if (fuel > 5.0f) {
            fuel_.setFillColor(sf::Color::Yellow);
        } else {
            fuel_.setFillColor(sf::Color::Red);
            fuel_.startBlinking(0.5f);
        }
        os.str("");
        os << "Gravity: " << std::setw(6) << std::setprecision(2) <<
               GRAVITY + gravity_modifier << " m/s^2";
        gravity_.setString(os.str());
    }
    fuel_.update(dt);
}

void
Altitude::acceptSystem(cbisf::ISystem& sys)
{
    sys.addEntity(gravity_);
    sys.addEntity(altitude_);
    sys.addEntity(velocity_);
    sys.addEntity(fuel_);
}

class InstructionState : public cbisf::State
{
public:
    InstructionState() = default;
    ~InstructionState() = default;

    bool draw(cbisf::Context&target, sf::RenderStates states) const override;
};

bool
InstructionState::draw(cbisf::Context &target, sf::RenderStates states) const
{
    static const char *text = R"(
Lunar Landing Simulator.

Use the space bar to apply thrust. When you are out of fuel, you will
not be able to counteract the pull of gravity.

Additionally, you may press the <A> key to attempt an abort of landing.
This will burn 5 units of fuel, (or all your fuel if you have less than 5
units remaining), and apply that as emergency thrust all at once.

You may use the <+> and <-> keys to modify the force of gravity.
<+> will increase gravity, while <-> will decrease it.

Finally, press the <P> key to pause the simulation.

Press <R>, now to run the simulation.
)";

    static sf::Text instructions(text, fontManager.get(Fonts::Vera), 15);
    target.window().draw(instructions, states);
    return lastDrawn_;
}

class LandedState : public cbisf::State
{
public:
    using cbisf::State::State;
    ~LandedState() = default;

    void setShip(const cbisf::IEntity &ship)     { ship_ = &ship; }

    bool draw(cbisf::Context &target, sf::RenderStates states) const override;

private:
    const cbisf::IEntity *ship_ = nullptr;
};

bool
LandedState::draw(cbisf::Context &target, sf::RenderStates states) const
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
    target.window().draw(text, states);
    return lastDrawn_;
}

class Thrust : public cbisf::CircleEntity
{
public:
    Thrust() : cbisf::CircleEntity(8.0f, 3) { }
    using inherited = cbisf::CircleEntity;

private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override;
};

void
Thrust::drawThis(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
        parent_->properties.get<float>("fuel") > 0.0f) {
        inherited::drawThis(target, states);
        //target.draw(circle_, states);
    }
}

class Lander
{
public:
    Lander() = default;
    ~Lander() = default;

    void init(cbisf::Engine &engine);

public:
    // Size of the world
    static constexpr float   height_ = 800.0f;
    static constexpr float   width_ = 600.0f;
    static constexpr float   maxFuel_ = 15.0f;

private:
    cbisf::CircleEntity      ship_{10.0f, 3};
    Thrust                   thrust_;
    cbisf::RectangleEntity   ground_{{width_, 100.0f}};
    cbisf::MovementSystem    ms_;
    cbisf::CollisionSystem   cs_{cbisf::CollisionSystem::AABB};
    cbisf::DrawingSystem     ds_;
    Altitude                 alt_;

    cbisf::State             landingState_;
    InstructionState         iState_;
    LandedState              landedState_{false, true, true};
    PausedState              pausedState_;

};

void
Lander::init(cbisf::Engine &engine)
{
    // Initialize the ship.
    ship_.setFillColor(sf::Color::Yellow);
    ship_.setOrigin(5.0f, 5.0f);
    ship_.setPosition(width_ / 2.0f, 10.0f);
    ship_.properties.add<float>("fuel", maxFuel_);

    ship_.addChild(thrust_);
    thrust_.setFillColor(sf::Color::Red);
    thrust_.setOrigin(4.0f, 4.0f);
    thrust_.rotate(180.0f);
    thrust_.setPosition(14.0f, 25.0f);

    ground_.setPosition(0.0f, height_ -10.0f);
    ground_.setFillColor(sf::Color::Red);

    landingState_.addSystem(ms_);
    landingState_.addSystem(cs_);
    landingState_.addSystem(ds_);

    iState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::R}},
    [this](const sf::Event &, cbisf::Context &context) {
        context.stack().pop();
        context.stack().push(landingState_);
        gravity_modifier = 0.0f;
    });

    landingState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::A}},
    [this](const sf::Event &, cbisf::Context &context) {
        auto &vel = ship_.properties.ref<sf::Vector2f>("velocity");
        auto &fuel = ship_.properties.ref<float>("fuel");
        auto burn = 5.0f;
        if (fuel < burn) {
            burn = fuel;
        }
        fuel -= burn;
        vel.y -= burn * 10.0;
    });

    landingState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::P}},
    [this](const sf::Event &, cbisf::Context &context) {
        context.stack().push(pausedState_);
    });

    landingState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Add}},
    [](const sf::Event &, cbisf::Context &context) {
        gravity_modifier += 0.5f;
    });

    landingState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Subtract}},
    [](const sf::Event &, cbisf::Context &context) {
        gravity_modifier -= 0.5f;
    });

    pausedState_.addEvent({sf::Event::KeyPressed, {sf::Keyboard::P}},
    [](const sf::Event &, cbisf::Context &context) {
        context.stack().pop();
    });


    auto &context = engine.addContext("lander", sf::seconds(1.0f / 60.0f),
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
    alt_.acceptSystem(ds_);
    alt_.setMaxAlt(height_ - 10.0f);

    // Setup the landing state.
    landingState_.addSystem(ms_);
    landingState_.addSystem(cs_);
    landingState_.addSystem(alt_);
    landingState_.addSystem(ds_);

    // Setup collision system
    cs_.addHandler<cbisf::CircleEntity, cbisf::RectangleEntity>(
    [this, &engine] (cbisf::CircleEntity &ship, cbisf::RectangleEntity &ground, const sf::FloatRect&) {
        if (ship.properties.get<float>("altitude") <= 0.0f) {
            engine.getContext("lander").stack().push(landedState_);
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
        gravity_modifier = 0.0f;
    });
}

int main()
{
    fontManager.load(Fonts::Vera, "C:/Users/rnewman/AppData/Local/Microsoft/Windows/Fonts/VeraSe.ttf");
    cbisf::Engine engine;
    Lander lander;

    lander.init(engine);
    engine.addThreads(3u);
    engine.run();
    return 0;
}
