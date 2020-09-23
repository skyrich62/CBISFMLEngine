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

int main()
{
    sf::RenderWindow app(sf::VideoMode(800, 600), "Demo Window");
    app.setFramerateLimit(60);

    cbisf::ResourceManager<Textures, sf::Texture> textureManager;
    if (!textureManager.load(Textures::Block, "cb.bmp")) {
        cbi::CheckPoint::hit(CBI_HERE, "Can't load cbi.bmp");
        return 5;
    }

    cbisf::ResourceManager<Fonts, sf::Font> fontManager;

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
    cbisf::State state;

    state.addSystem(ms);
    state.addSystem(ss);
    state.addSystem(cs);
    state.addSystem(ds);
    state.addSystem(rs);

    engine.stack().push(state);

    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Up}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") += 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Down}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") -= 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::PageUp}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") -= 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::PageDown}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") += 10.0f;
               } );
    engine.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Right}},
               [&child](const sf::Event &e) {
                   child.move(10.0f, 0.0f);
               } );
    engine.addEvent({sf::Event::KeyPressed, {sf::Keyboard::Left}},
               [&child](const sf::Event &e) {
                   child.move(-10.0f, 0.0f);
               } );
    engine.addEvent({sf::Event::Closed},
               [&app](const sf::Event &) {
                   app.close();
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Escape}},
               [&app](const sf::Event &) {
                    app.close();
                });
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Space}},
               [&entity,&child](const sf::Event &) {
                    entity.properties.set<float>("rotation", 0.0f);
                    child.properties.set<float>("rotation", 0.0f);
                });

    engine.run(app);

    return EXIT_SUCCESS;
}
