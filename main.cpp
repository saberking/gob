

#include "includes.hpp"

sf::Font font;

 sf::RenderWindow window(sf::VideoMode(1200, 700), "Gob");






void setup(){
 
    srand(time(NULL));
        font.loadFromFile("fonts/thestrong.ttf");
  
}

int main(){
    setup();


    sf::Clock clock;
    sf::Time elapsed;
    sf::Event event;

    window.setFramerateLimit(20);
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::MouseButtonPressed){
                sf::Vector2i pos=sf::Mouse::getPosition(window);

            }
        }
        elapsed = clock.getElapsedTime();

    }

    return 0;
}