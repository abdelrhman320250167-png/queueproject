#ifndef QUEUE_H
#define QUEUE_H

#include "Customer.h"
#include <deque>

class Queue {
private:
    std::deque<Customer*> customers;

public:
    void enqueue(Customer* c);
    Customer* dequeue();
    bool isEmpty() const;
    void updateWaitingTimes();
};

#endif
