#include "Server.h"
#include <iostream>

using namespace std;

Server::Server() {
    currentCustomer = nullptr;
    busy = false;
    remainingTime = 0;
    servedCount = 0;
}

bool Server::isFree() const { return !busy; }
bool Server::isBusy() const { return busy; }

void Server::assignCustomer(Customer* c) {
    currentCustomer = c;
    busy = true;
    remainingTime = c->getTransactionTime();
    c->setStatus(SERVING);
}

void Server::updateTime() {
    if (busy && currentCustomer != nullptr) {
        remainingTime--;
        if (remainingTime <= 0) {
            finishService();
        }
    }
}

void Server::finishService() {
    if (currentCustomer != nullptr) {
        currentCustomer->setStatus(DONE);
        currentCustomer = nullptr;
        busy = false;
        servedCount++;
    }
}

Customer* Server::getCurrentCustomer() const { return currentCustomer; }
int Server::getRemainingTime() const { return remainingTime; }
int Server::getTotalServedCount() const { return servedCount; }

void Server::printStatus() const {
    if (busy) cout << "Busy (" << remainingTime << " left)";
    else cout << "Free";
}
