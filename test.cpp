#include <CompuBrite/SFML/Entity.h>
#include <CompuBrite/SFML/ResourceManager.h>
#include <CompuBrite/SFML/EventManager.h>
#include <compubrite/checkpoint.h>

#include <SFML/Graphics.hpp>

namespace cbisf = CompuBrite::SFML;
namespace cbi = CompuBrite;

enum class Textures {Block};

class Block : public cbisf::Entity
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

    void updateThis(sf::Time dt)
    {
        this->rotate(rotation_ * dt.asSeconds());
    }

    float &rotation() { return rotation_; }
private:
    void drawThis(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(sprite_, states);
    }
private:
    sf::Sprite sprite_;
    float rotation_ = 0.0f;
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

    cbisf::EventManager events;

    Block entity(textureManager.get(Textures::Block));
    Block child(textureManager.get(Textures::Block));

    auto size = entity.getLocalBounds();
    entity.setOrigin(size.width / 2.0f, size.height / 2.0f);
    entity.move(400.0f, 300.0f);
    child.setOrigin(size.width / 2.0f, size.height / 2.0f);
    child.move(size.width* 1.5f, size.height * 1.5f);
    entity.addChild(child);


    events.add({sf::Event::KeyReleased, {sf::Keyboard::Up}},
               [&entity](const sf::Event &e) {
                   entity.rotation() += 10.0f;
               } );
    events.add({sf::Event::KeyReleased, {sf::Keyboard::Down}},
               [&entity](const sf::Event &e) {
                   entity.rotation() -= 10.0f;
               } );
    events.add({sf::Event::KeyReleased, {sf::Keyboard::Left}},
               [&child](const sf::Event &e) {
                   child.rotation() += 10.0f;
               } );
    events.add({sf::Event::KeyReleased, {sf::Keyboard::Right}},
               [&child](const sf::Event &e) {
                   child.rotation() -= 10.0f;
               } );
    events.add({sf::Event::Closed},
               [&app](const sf::Event &) {
                   app.close();
               } );
    events.add({sf::Event::KeyReleased, {sf::Keyboard::Escape}},
               [&app](const sf::Event &) {
                    app.close();
                });

    cbi::CheckPoint dbg("entity");

    sf::Clock clock;
	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            events.dispatch(event);
        }

        // Clear screen
        app.clear();

        entity.update(clock.restart());

        // Draw the sprite
        app.draw(entity);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
