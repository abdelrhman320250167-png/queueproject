#include "Controller.h"

// main.cpp
// Entry point for the simulation.

int main() {
    Controller systemController;
    systemController.initializeSystem();
    systemController.startSimulation();
    return 0;
}