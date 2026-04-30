#include "Customer.h"
#include <iostream>

Customer::Customer(int id, int arrTime, int transTime, bool vip) {
    this->id = id;
    this->arrivalTime = arrTime;
    this->transactionTime = transTime;
    this->isVIP = vip;
    this->waitingTime = 0;
    this->status = WAITING;
}

int Customer::getId() const {
    return id;
}

int Customer::getArrivalTime() const {
    return arrivalTime;
}

int Customer::getTransactionTime() const {
    return transactionTime;
}

int Customer::getWaitingTime() const {
    return waitingTime;
}

bool Customer::getIsVIP() const {
    return isVIP;
}

Status Customer::getStatus() const {
    return status;
}

void Customer::setWaitingTime(int time) {
    waitingTime = time;
}

void Customer::calculateWaitingTime(int currentTime) {
    waitingTime = currentTime - arrivalTime;
}

void Customer::setStatus(Status s) {
    status = s;
}

void Customer::printInfo() const {
    std::cout << "Customer ID: " << id 
              << " | Arrival: " << arrivalTime 
              << " | Transaction: " << transactionTime 
              << " | VIP: " << (isVIP ? "Yes" : "No") 
              << " | Waiting Time: " << waitingTime << std::endl;
}
