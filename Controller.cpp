#include "Controller.h"
#include <SFML/System.hpp>

// إعطاء قيم ابتدائية للمحاكاة (مثلا 5 سيرفرات و 500 وقت)
Controller::Controller() : sim(5, 500) {
}

void Controller::initializeSystem() {
    // الواجهة والمحاكاة بيتعملهم Initialize تلقائياً دلوقتي
}

void Controller::startSimulation() {
    sf::Clock clock;
    float tickRate = 0.5f; // سرعة المحاكاة: كل نص ثانية 

    while (gui.isOpen()) {
        
        gui.handleEvents(); 

        if (sim.getIsRunning()) {
            // تحديث المحاكاة كل نص ثانية
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
