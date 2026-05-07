#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;

public:
    GUI();
    
    bool isOpen() const;
    void handleEvents();
    void render(const Simulation& sim);
    void close();
};

#endif
