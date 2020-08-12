#include <CompuBrite/SFML/IEntity.h>
#include <CompuBrite/SFML/ResourceManager.h>
#include <CompuBrite/SFML/EventManager.h>
#include <CompuBrite/SFML/TProperty.h>
#include <CompuBrite/SFML/MovementSystem.h>
#include <CompuBrite/SFML/DrawingSystem.h>
#include <CompuBrite/SFML/Engine.h>
#include <CompuBrite/CheckPoint.h>

#include <SFML/Graphics.hpp>

namespace cbisf = CompuBrite::SFML;
namespace cbi = CompuBrite;

enum class Textures {Block};

class Block : public cbisf::IEntity
{
public:
    Block(const sf::Texture &texture) :
        sprite_(texture)
    {
    }

    sf::FloatRect getLocalBounds() const
    {
        return sprite_.getLocalBounds();
    }

private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(sprite_, states);
    }
private:
    sf::Sprite sprite_;
};

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "SFML window");
    app.setFramerateLimit(60);

    cbisf::ResourceManager<Textures, sf::Texture> textureManager;
    if (!textureManager.load(Textures::Block, "cb.bmp")) {
        cbi::CheckPoint::hit(CBI_HERE, "Can't load cbi.bmp");
        return 5;
    }

    Block entity(textureManager.get(Textures::Block));
    Block child(textureManager.get(Textures::Block));

    auto size = entity.getLocalBounds();
    entity.setOrigin(size.width / 2.0f, size.height / 2.0f);
    entity.move(400.0f, 300.0f);
    child.setOrigin(size.width / 2.0f, size.height / 2.0f);
    child.move(size.width* 1.5f, size.height * 1.5f);
    entity.addChild(child);

    cbisf::MovementSystem ms;
    ms.addEntity(entity);
    ms.addEntity(child);

    cbisf::DrawingSystem ds;
    ds.addEntity(entity);

    cbisf::Engine engine;
    engine.addSystem(ms);
    engine.addSystem(ds);

    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Up}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") += 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Down}},
               [&entity](const sf::Event &e) {
                   entity.properties.ref<float>("rotation") -= 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Left}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") += 10.0f;
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Right}},
               [&child](const sf::Event &e) {
                   child.properties.ref<float>("rotation") -= 10.0f;
               } );
    engine.addEvent({sf::Event::Closed},
               [&app](const sf::Event &) {
                   app.close();
               } );
    engine.addEvent({sf::Event::KeyReleased, {sf::Keyboard::Escape}},
               [&app](const sf::Event &) {
                    app.close();
                });

    cbi::CheckPoint dbg("entity");


    engine.run(app);

    return EXIT_SUCCESS;
}
