#include "Controller.h"

int main() {
    Controller controller;
    
    // استدعاء دالة التهيئة ثم بدء المحاكاة
    controller.initializeSystem();
    controller.startSimulation();
    
    return 0;
}
