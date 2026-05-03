#include "Queue.h"
#include <iostream>

using namespace std;

void Queue::enqueueCustomer(Customer* c) {
    if (c->getIsVIP()) {
        customers.push_front(c);
    } else {
        customers.push_back(c);
    }
}

Customer* Queue::dequeueCustomer() {
    if (!isEmpty()) {
        Customer* frontC = customers.front();
        customers.pop_front();
        return frontC;
    }
    return nullptr; 
}

bool Queue::isEmpty() const {
    return customers.empty();
}

void Queue::updateWaitingTimes(int currentTime) {
    for (int i = 0; i < (int)customers.size(); i++) {
        customers[i]->updateWaitingDuration(currentTime);
    }
}

int Queue::getQueueSize() const {
    return (int)customers.size();
}

Customer* Queue::getCustomerAt(int index) const {
    if (index >= 0 && index < (int)customers.size()) {
        return customers[index];
    }
    return nullptr;
}
