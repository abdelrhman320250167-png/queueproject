#ifndef QUEUE_H
#define QUEUE_H

#include "Customer.h"
#include <deque>

using namespace std;

class Queue {
private:
    deque<Customer*> customers;

public:
    void enqueue(Customer* c);
    Customer* dequeue();
    bool isEmpty() const;
    void updateWaitingTimes();
};

#endif
