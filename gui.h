#ifndef SIMULATION_GUI_H
#define SIMULATION_GUI_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"

class SimulationGUI {
private:
    // 1. الأساسيات
    Simulation& sim;             // reference for simulation to get any new edit
    sf::RenderWindow window;     
    sf::Font font;               //FONT TYOE

    // edit size
    const int windowWidth = 1200; 
    const int windowHeight = 700;
    
    
    sf::Color bgColor;           
    sf::Color normalCustomerColor;  
    sf::Color vipCustomerColor;    
    sf::Color freeServerColor;     
    sf::Color busyServerColor;    


    
    float tickRate;
    sf::Clock clock;

    
    void handleEvents();// deal with 
    void update();
    void render();

    void drawQueue();           
    void drawDashboard();        
    void drawGraph();
    void drawServers();

public:
    
    SimulationGUI(Simulation& s);

    
    void run();
};

#endif