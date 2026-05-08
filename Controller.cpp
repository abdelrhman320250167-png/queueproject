#include "Controller.h"
#include <SFML/System.hpp>

Controller::Controller() : sim(5, 500) {
}

void Controller::initializeSystem() {
}

void Controller::startSimulation() {
    sf::Clock clock;
    float tickRate = 0.5f;   

    while (gui.isOpen()) {
        
        gui.handleEvents(); 

        if (sim.getIsRunning()) {
        
            if (clock.getElapsedTime().asSeconds() >= tickRate) {
                updateSystem();
                clock.restart(); 
            }
        }

        gui.render(sim); 
    }
}

void Controller::updateSystem() {
    sim.runTick(); 
}
