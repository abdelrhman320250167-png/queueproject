#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Simulation.h"

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Clock animClock;
    std::vector<int> visualHistory;
    float tickRate;
    Simulation& sim;

    sf::Color bgColor;
    sf::Color normalCustomerColor;
    sf::Color vipCustomerColor;
    sf::Color freeServerColor;
    sf::Color busyServerColor;

    void drawDashboard();
    void drawQueue();
    void drawServers();
    void drawGraph();

public:
    GUI(Simulation& s);
    void render(Simulation& s);
    void handleEvents();
    bool isOpen() const;
    void run();
};

#endif