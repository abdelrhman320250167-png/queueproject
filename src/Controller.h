#pragma once

// Controller.h
// Defines the Controller class coordinating the Simulation and GUI.

#include <SFML/System.hpp>
#include "Simulation.h"
#include "Gui.h" // Note: Make sure this matches your actual file name casing

class Controller {
private:
    Simulation simulation; // Core simulation instance
    Gui gui;               // Graphical User Interface instance
    sf::Clock tickClock;   // Clock to regulate simulation updates
    float timePerTick;     // Duration of a simulation tick in seconds

public:
    // Constructor initializes simulation parameters and links GUI
    Controller()
        : simulation(3, 100),
          gui(&simulation),
          timePerTick(0.5f) {}

    // Resets simulation state and initializes the GUI window
    void initializeSystem() {
        simulation.initializeSimulation();
        gui.initializeWindow();
    }

    // Main loop: handles events, updates simulation based on time, and refreshes display
    void startSimulation() {
        while (gui.isOpen()) {
            gui.handleEvents();
            
            // Advance simulation only if running and enough real time has passed
            if (simulation.getIsRunning()) {
                if (tickClock.getElapsedTime().asSeconds() >= timePerTick) {
                    updateSystem();
                    tickClock.restart();
                }
            }
            
            gui.refreshDisplay();
        }
    }

    // Advances the simulation by one tick
    void updateSystem() {
        simulation.updateSimulation();
    }
};