#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Simulation.h"

struct AnimPos {
    float x, y;
};

class GUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Clock animClock;
    std::vector<int> visualHistory;
    Simulation& sim;

    std::map<int, AnimPos> customerPositions;

    sf::Color bgColor;
    sf::Color normalCustomerColor;
    sf::Color vipCustomerColor;
    sf::Color freeServerColor;
    sf::Color busyServerColor;

    void drawDashboard();
    void drawQueue();
    void drawServers();
    void drawGraph();
    void drawGlowCircle(float x, float y, float radius, sf::Color color, int intensity);

public:
    GUI(Simulation& s);
    void render(Simulation& s);
    void handleEvents();
    bool isOpen() const;
};

#endif