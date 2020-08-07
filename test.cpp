#include <CompuBrite/SFML/Entity.h>
#include <compubrite/checkpoint.h>

#include <SFML/Graphics.hpp>

namespace cbisf = CompuBrite::SFML;
namespace cbi = CompuBrite;

class Block
{
public:
    Block(const std::string &filename)
    {
        if (!texture_.loadFromFile(filename)) {
            cbi::CheckPoint::hit(CBI_HERE, "Can't load ", filename);
            return;
        }
        sprite_.setTexture(texture_);
    }

    cbisf::EntityImpl impl{
        [this](sf::RenderTarget& target, sf::RenderStates states)
        {
            target.draw(sprite_, states);
        },
        [this]() { return sprite_.getLocalBounds(); }
    };
private:
    sf::Sprite sprite_;
    sf::Texture texture_;
};

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "SFML window");
    app.setFramerateLimit(60);

    Block block("cb.bmp");
    cbisf::Entity entity(block.impl);
    auto size = entity.getLocalBounds();
    entity.setOrigin(size.width / 2.0, size.height / 2.0);
    entity.move(400.0, 300.0);

    cbi::CheckPoint dbg("entity");

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }
        entity.rotate(1.0);

        // Clear screen
        app.clear();

        // Draw the sprite
        app.draw(entity);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
