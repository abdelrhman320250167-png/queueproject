#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "simulation.h" 
#include "gui.h"

class Controller {
private:
    Simulation sim;
    GUI gui; 

public:
    Controller();
    void initializeSystem();
    void startSimulation();
    void updateSystem();
};

#endif
