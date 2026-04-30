#include "Server.h"

using namespace std;

Server::Server() {
    currentCustomer = nullptr;
    busy = false;
    remainingTime = 0;
}

bool Server::isFree() const {
    return !busy;
}

void Server::assignCustomer(Customer* c) {
    if (c == nullptr) return;

    currentCustomer = c;
    busy = true;
    remainingTime = c->getTransactionTime();
    c->setStatus(SERVING);
}

void Server::updateTime() {
    if (!busy || currentCustomer == nullptr) return;

    remainingTime--;

    if (remainingTime <= 0) {
        finishService();
    }
}

void Server::finishService() {
    if (currentCustomer != nullptr) {
        currentCustomer->setStatus(DONE);
    }

    currentCustomer = nullptr;
    busy = false;
    remainingTime = 0;
}

Customer* Server::getCurrentCustomer() const {
    return currentCustomer;
}
