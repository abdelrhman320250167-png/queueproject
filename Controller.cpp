#include "Controller.h"
#include <SFML/System.hpp>
#include <ctime>
#include <cstdlib>

Controller::Controller() : sim(3, 10000), gui(sim) {
    srand(time(0));
}

void Controller::initializeSystem() {}

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
    if (rand() % 100 < 35) { 
        int sequentialID = sim.getArrivedCount() + 1; 
        int randomServiceTime = rand() % 10 + 5; 
        bool isVIP = (rand() % 10 == 0); 
        
        Customer* c = new Customer(sequentialID, sim.getCurrentTime(), randomServiceTime, isVIP);
        
        sim.addCustomerExternal(c);
    }
    sim.runTick(); 
}
