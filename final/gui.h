#pragma once

// Gui.h
// Provides a modern, animated graphical interface for the queue simulation using SFML 3.x.

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include "Simulation.h"

class Gui {
private:
    Simulation* simulationRef;
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock animClock;
    sf::RectangleShape pauseButton;
    sf::Text pauseText;

    void drawDashboard();
    void drawServers();
    void drawQueue();
    void drawGraph();

public:
    Gui(Simulation* sim);

    void initializeWindow();
    void connectToSimulation(Simulation* sim);
    bool isOpen() const;
    void closeWindow();

    void handleEvents();
    void refreshDisplay();
};