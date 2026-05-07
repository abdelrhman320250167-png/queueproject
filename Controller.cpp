#include "Controller.h"
#include <iostream>

Controller::Controller() : sim(5) { 
}

void Controller::startSimulation() {
    int ticks = 0;
    while (ticks < 10) { 
        sim.runTick(); 
        std::cout << "Time is now: " << sim.getCurrentTime() << std::endl;
        ticks++;
    }
}