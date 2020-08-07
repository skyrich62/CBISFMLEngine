#include <CompuBrite/SFML/Entity.h>
#include <compubrite/checkpoint.h>

#include <SFML/Graphics.hpp>

namespace cbisf = CompuBrite::SFML;

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "SFML window");
    app.setFramerateLimit(60);

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    cbisf::EntityImpl impl{
        [&sprite](sf::RenderTarget& target, sf::RenderStates states)
        {
            target.draw(sprite, states);
        },
        [&sprite]() { return sprite.getLocalBounds(); }
    };
    cbisf::Entity entity(impl);
    auto size = entity.getLocalBounds();
    entity.setOrigin(size.width / 2.0, size.height / 2.0);
    entity.move(400.0, 300.0);

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
