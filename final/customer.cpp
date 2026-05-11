#include "Customer.h"

// Constructor with default values
Customer::Customer(int id_, int arrTime, int serviceTime_, bool vip)
    : id(id_),
    arrivalTime(arrTime),
    serviceTime(serviceTime_),
    waitingTime(0),
    isVIP(vip),
    status(Status::WAITING) {}

// Getters
int Customer::getId() const { return id; }
int Customer::getArrivalTime() const { return arrivalTime; }
int Customer::getServiceTime() const { return serviceTime; }
int Customer::getWaitingTime() const { return waitingTime; }
bool Customer::getIsVIP() const { return isVIP; }
Status Customer::getStatus() const { return status; }

// Setters
void Customer::setWaitingTime(int time) { waitingTime = time; }
void Customer::setStatus(Status s) { status = s; }

// Updates waiting time based on current time (only if still waiting)
void Customer::calculateWaitingTime(int currentTime) {
    if (status == Status::WAITING) {
        waitingTime = currentTime - arrivalTime;
    }
}