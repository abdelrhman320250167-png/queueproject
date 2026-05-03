#include "Queue.h"
#include <iostream>

using namespace std;

void Queue::enqueueCustomer(Customer* c) {
    if (c->getIsVIP()) {
        auto it = customers.begin();
        while (it != customers.end() && (*it)->getIsVIP()) {
            ++it;
        }
        customers.insert(it, c); 
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
    for (size_t i = 0; i < customers.size(); i++) {
        customers[i]->calculateWaitingTime(currentTime); 
    }
}

int Queue::getQueueSize() const {
    return customers.size();
}

Customer* Queue::getCustomerAt(size_t index) const {
    if (index < customers.size()) {
        return customers[index];
    }
    return nullptr;
}
