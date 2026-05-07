#ifndef QUEUE_H
#define QUEUE_H

#include "Customer.h"
#include <deque>

using namespace std;

class Queue {
private:
    deque<Customer*> customers;

public:
    void enqueueCustomer(Customer* c);
    Customer* dequeueCustomer();
    bool isEmpty() const;
    void updateWaitingTimes(int currentTime);
    
    int getQueueSize() const;
    Customer* getCustomerAt(int index) const;
};

#endif
