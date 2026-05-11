#include "Controller.h"

// Constructor initializes simulation parameters and links GUI
Controller::Controller()
    : simulation(3, 100),
    gui(&simulation),
    timePerTick(0.5f) {}

// Resets simulation state and initializes the GUI window
void Controller::initializeSystem() {
    simulation.initializeSimulation();
    gui.initializeWindow();
}

// Main loop: handles events, updates simulation based on time, and refreshes display
void Controller::startSimulation() {
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
void Controller::updateSystem() {
    simulation.updateSimulation();
}