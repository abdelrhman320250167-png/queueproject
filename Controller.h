#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Simulation.h" 
#include "GUI.h"

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
