#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Err.hpp>
#include "board.hpp"
#include "game.hpp"
#include "controller.hpp"

using namespace std;


int main()
{
    Size size{600,600};

    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = true;

    sf::RenderWindow window(sf::VideoMode(size.x,size.y), "Test", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    Controller white,
               black; //Чтобы играть двум игрокам

    AINo2 ai2;
    PawnGame game(size, &white, &ai2);

    // Start game loop
    game.start();
    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }

            if((event.mouseButton.button == sf::Mouse::Left) && (event.type == sf::Event::MouseButtonPressed))
            {
                if((0 <= event.mouseButton.x) && (event.mouseButton.x <= size.x) &&
                   (0 <= event.mouseButton.y) && (event.mouseButton.y <= size.y))
                {
                    game.getCurrentController()->mouseEvent({event.mouseButton.x,event.mouseButton.y});
                }
            }

            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }
        window.draw(game);
        window.display();
    }


    cout << "Hello World!" << endl; //Hello world
    return 0;
}
