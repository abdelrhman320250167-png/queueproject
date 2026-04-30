#include "Customer.h"

using namespace std;

Customer::Customer(int id, int arrTime, int transTime, bool vip) {
    this->id = id;
    arrivalTime = arrTime;
    transactionTime = transTime;
    waitingTime = 0;
    isVIP = vip;
    status = WAITING;
}

int Customer::getId() const { return id; }
int Customer::getArrivalTime() const { return arrivalTime; }
int Customer::getTransactionTime() const { return transactionTime; }
int Customer::getWaitingTime() const { return waitingTime; }
bool Customer::getIsVIP() const { return isVIP; }
Status Customer::getStatus() const { return status; }

void Customer::setWaitingTime(int time) { waitingTime = time; }
void Customer::incrementWaitingTime() { waitingTime++; }
void Customer::setStatus(Status s) { status = s; }
