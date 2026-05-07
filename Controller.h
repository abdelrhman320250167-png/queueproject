#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "simulation.h" 
#include "gui.h"        

class Controller {
private:
    Simulation sim;  

public:
    Controller(); 
    void startSimulation();  
};

#endif