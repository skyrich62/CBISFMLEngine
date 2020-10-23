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
 * @brief A sample application using the Engine.
*/

#include <CompuBrite/SFML/SpriteEntity.h>
#include <CompuBrite/SFML/TextEntity.h>
#include <CompuBrite/SFML/RectangleEntity.h>
#include <CompuBrite/SFML/CircleEntity.h>
#include <CompuBrite/SFML/ResourceManager.h>
#include <CompuBrite/SFML/EventManager.h>
#include <CompuBrite/SFML/TProperty.h>
#include <CompuBrite/SFML/MovementSystem.h>
#include <CompuBrite/SFML/DrawingSystem.h>
#include <CompuBrite/SFML/CollisionSystem.h>
#include <CompuBrite/SFML/Engine.h>
#include <CompuBrite/SFML/State.h>
#include <CompuBrite/CheckPoint.h>

#include <SFML/Graphics.hpp>

namespace cbisf = CompuBrite::SFML;
namespace cbi = CompuBrite;

enum class Textures {Block};
enum class Fonts {Vera};

auto offset = 100.0f;
auto level = cbisf::CollisionSystem::SAT;

cbisf::ResourceManager<Fonts, sf::Font> fontManager;

class StatusSystem : public cbisf::ISystem
{
public:
    StatusSystem(cbisf::TextEntity &status) :
        status_(status),
        elapsed_(sf::Time::Zero)
    { }
    ~StatusSystem() = default;

    void addProperties(cbisf::IEntity &) override;

    void update(sf::Time dt) override;

    bool collision_ = false;
private:
    cbisf::TextEntity &status_;
    sf::Time elapsed_;
};

void
StatusSystem::addProperties(cbisf::IEntity &entity)
{
    entity.properties.add<std::string>("name");
}

void
StatusSystem::update(sf::Time dt)
{

    elapsed_ += dt;
    //cbi::CheckPoint::hit(CBI_HERE, "dt = ", dt.asMicroseconds());
    if (elapsed_.asSeconds() < 0.1f) {
        return;
    }
    elapsed_ = sf::Time::Zero;
    std::ostringstream os;
    os << "Status:\n";
    for (auto entity : entities_) {
        os << entity->properties.get<std::string>("name") << ", pos: ";
        auto pos = entity->getGlobalPosition();
        os << pos.x << ", " << pos.y << ", rot: ";
        os << entity->getRotation();
        auto bounds = entity->getGlobalBounds();
        os << ", Global: " << bounds.top << ", " << bounds.height << ", "
                           << bounds.left << ", " << bounds.width;
        os << '\n';
    }
    if (collision_) {
        os << "** COLLISION **\n";
        collision_ = false;
    }
    status_.setString(os.str());
}

class ResetColorSystem : public cbisf::ISystem
{
public:
    ResetColorSystem() = default;
    ~ResetColorSystem() = default;

    void update(sf::Time dt);
};

void
ResetColorSystem::update(sf::Time dt)
{
    for (auto entity : entities_) {
        auto ptr = dynamic_cast<cbisf::IShapeEntity*>(entity);
        if (ptr) {
            ptr->setFillColor(sf::Color::White);
        }
    }
}

class BlinkingSystem : public cbisf::ISystem
{
public:
    BlinkingSystem() = default;
    ~BlinkingSystem() = default;
    void update(sf::Time dt);
    bool blink() const                      { return blink_; }
private:
    sf::Time elapsed_;
    bool     blink_;
};



class PausedState : public cbisf::State
{
public:
    PausedState();
    ~PausedState() = default;

    bool update(sf::Time dt) override;
    bool draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Time             elapsed_ = sf::Time::Zero;
    bool                 blink_ = true;
};

PausedState::PausedState() :
    State(false, true, true)
{
}

bool
PausedState::update(sf::Time dt)
{
    elapsed_ += dt;
    if (elapsed_.asSeconds() > 0.5f) {
        elapsed_ = sf::Time::Zero;
        blink_ = !blink_;
    }
    return true;
}

bool
PausedState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static cbisf::RectangleEntity background;
    static cbisf::TextEntity paused("Paused - Press 'P' to continue",
                                    fontManager.get(Fonts::Vera), 25);
    static auto once = true;

    if (once) {
        background.setFillColor({0, 0, 0, 127});
        sf::Vector2f size(target.getSize());
        background.setSize(size);
        background.setOrigin(size / 2.0f);
        background.setPosition(size / 2.0f);
        paused.setFillColor(sf::Color::Red);
        auto bounds = paused.getLocalBounds();
        sf::Vector2f sizeText(bounds.width, bounds.height);
        paused.setOrigin(sizeText / 2.0f);
        paused.setPosition(size / 2.0f);
        once = false;
    }

    target.draw(background);
    if (blink_) {
        target.draw(paused);
    }
    return false;
}

int main()
{
    cbisf::ResourceManager<Textures, sf::Texture> textureManager;
    if (!textureManager.load(Textures::Block, "cb.bmp")) {
        cbi::CheckPoint::hit(CBI_HERE, "Can't load cbi.bmp");
        return 5;
    }

    if (!fontManager.load(Fonts::Vera, "C:/Users/rnewman/AppData/Local/Microsoft/Windows/Fonts/VeraSe.ttf")) {
        cbi::CheckPoint::hit(CBI_HERE, "Can't load Vera font");
        return 6;
    }

    cbisf::SpriteEntity entity(1, textureManager.get(Textures::Block));
    cbisf::SpriteEntity child(1, textureManager.get(Textures::Block));

    auto size = entity.getLocalBounds();
    entity.setOrigin(size.width / 2.0f, size.height / 2.0f);
    entity.setPosition(400.0f, 300.0f);

    auto pos = entity.getGlobalPosition();

    child.setOrigin(size.width / 2.0f, size.height / 2.0f);
    child.setPosition(size.width + offset, size.height / 2.0f);
    entity.addChild(child);
    pos = child.getGlobalPosition();

    cbisf::TextEntity status("***", fontManager.get(Fonts::Vera), 15);
    StatusSystem ss(status);
    ss.addEntity(entity);
    ss.addEntity(child);
    entity.properties.set<std::string>("name", "parent");
    child.properties.set<std::string>("name", "child");

    cbisf::MovementSystem ms;
    ms.addEntity(entity);
    ms.addEntity(child);

    cbisf::DrawingSystem ds;
    ds.addEntity(entity);
    ds.addEntity(status);

    cbisf::CollisionSystem cs(level);
    cs.addHandler<cbisf::SpriteEntity, cbisf::SpriteEntity>(
        [&ss](cbisf::SpriteEntity &, cbisf::SpriteEntity &, const sf::FloatRect &) {
            ss.collision_ = true;
        });
    cs.addEntity(entity);
    cs.addEntity(child);

    cbisf::RectangleEntity rect({50.f, 50.f});
    rect.setOrigin(10.f, 10.f);
    rect.setPosition(200.f, 200.f);
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(5.0f);
    ds.addEntity(rect);
    cs.addHandler<cbisf::SpriteEntity, cbisf::RectangleEntity>(
        [&rect, &ss] (cbisf::SpriteEntity &, cbisf::RectangleEntity &, const sf::FloatRect &)
        {
            rect.setFillColor(sf::Color::Red);
            ss.collision_ = true;
        });
    cs.addEntity(rect);

    cbisf::CircleEntity circle(10.f);
    circle.zOrder(10);
    circle.setOrigin(5.0f, 5.0f);
    circle.setPosition(400.f, 400.f);
    circle.setFillColor(sf::Color::Red);
    circle.setOutlineColor(sf::Color::Yellow);
    circle.setOutlineThickness(-4.f);
    ds.addEntity(circle);

    ResetColorSystem rs;
    rs.addEntity(rect);

    cbisf::Engine engine;
    cbisf::State mainState;
    PausedState pausedState;

    mainState.addSystem(ms);
    mainState.addSystem(ss);
    mainState.addSystem(cs);
    mainState.addSystem(ds);
    mainState.addSystem(rs);

    // Up key causes the child to rotate clockwise.
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Up}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") += 10.0f;
               } );

    // Down key causes the child to rotate counter-clockwise
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Down}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") -= 10.0f;
               } );

    // PageUp causes the main entity to rotate counter-clockwise
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::PageUp}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") -= 10.0f;
               } );

    // PageDown causes the main entity to rotate clockwise.
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::PageDown}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") += 10.0f;
               } );

    // Right arrow key causes the child to move to the right along the relative
    // x axis.  (This will be affected by main entity rotation)
    mainState.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Right}},
               [&child](const sf::Event &e) {
                   child.move(10.0f, 0.0f);
               } );

    // Left arrow key causes the child to move to the left along the relative
    // x axis, (This will be affected by main entity rotation)
    mainState.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Left}},
               [&child](const sf::Event &e) {
                   child.move(-10.0f, 0.0f);
               } );

    // Space bar will stop all rotations.
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Space}},
               [&entity,&child](const sf::Event &) {
                    entity.properties.set<float>("rotation", 0.0f);
                    child.properties.set<float>("rotation", 0.0f);
                } );

    // P key will cause a pause screen to overlay the main
    mainState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::P}},
               [&engine, &pausedState](const sf::Event &) {
                       engine.stack().push(pausedState);
                } );

    // P key while paused will resume normal operations.
    pausedState.addEvent({sf::Event::KeyReleased, {sf::Keyboard::P}},
                [&engine] (const sf::Event &) {
                         engine.stack().pop();
                } );

    // Closing the app window will cause the engine to stop by clearing the
    // state stack.
    engine.addEvent({sf::Event::Closed},
               [&engine](const sf::Event &) {
                   engine.stack().clear();
               } );

    // ESC key will cause the engine to stop by clearing the state stack.
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Escape}},
               [&engine](const sf::Event &) {
                    engine.stack().clear();
                } );

    // Push the main state.
    engine.stack().push(mainState);

    // Create an application window and throttle the frame rate.
    sf::RenderWindow app(sf::VideoMode(800, 600), "Demo Window");

    // Run the engine on the application window.
    engine.run(app, sf::seconds(1.0f / 60.0f));

    return EXIT_SUCCESS;
}
