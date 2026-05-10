#include "Controller.h"
#include "Customer.h"
#include "Queue.h"
#include <SFML/System.hpp>
#include <ctime>
#include <cstdlib>

// العداد المضمون للترتيب 1, 2, 3
static int customerCounter = 1;

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
    // احتمالية الدخول 35% عشان السيستم يكون هادي ومنظم
    if (rand() % 100 < 35) { 
        int sequentialID = customerCounter++; 
        int randomServiceTime = rand() % 10 + 5; 
        bool isVIP = false; 
        
        Customer* c = new Customer(sequentialID, sim.getCurrentTime(), randomServiceTime, isVIP);
        
        Queue& nonConstQueue = const_cast<Queue&>(sim.getQueue());
        nonConstQueue.enqueueCustomer(c);
    }
    sim.runTick(); 
}