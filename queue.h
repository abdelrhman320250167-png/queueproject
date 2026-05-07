#ifndef QUEUE_H
#define QUEUE_H

#include "Customer.h"

struct Node {
    Customer* data;
    Node* next;
    Node(Customer* c) : data(c), next(nullptr) {}
};

class Queue {
private:
    Node* front;
    Node* rear;
    int size;

public:
    Queue();
    ~Queue();
    
    void enqueueCustomer(Customer* c);
    Customer* dequeueCustomer();
    bool isEmpty() const;
    void updateWaitingTimes(int currentTime);

    int getQueueSize() const;
    Customer* getCustomerAt(int index) const;
};

#endif
