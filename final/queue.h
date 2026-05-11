#pragma once

#include "Customer.h"

// Node structure to represent each element in the linked list
struct Node {
    Customer* customer;
    Node* next;
    
    Node(Customer* c);
};

// Queue.h
// Defines a custom queue using a Linked List for the simulation.

class Queue {
private:
    Node* front; // Pointer to the front of the queue
    Node* rear;  // Pointer to the rear of the queue
    int count;   // Current number of elements in the queue

    // Private helper method to append a node directly (used in Copy/Assignment)
    void appendNode(Customer* c);

    // Private helper method to clear the linked list
    void clear();

public:
    // Default Constructor initializes an empty queue
    Queue();

    // Copy Constructor for deep copy of the queue structure
    Queue(const Queue& other);

    // Assignment Operator
    Queue& operator=(const Queue& other);

    // Destructor to free dynamically allocated nodes
    ~Queue();

    // Adds a customer to the queue. VIPs are inserted at the front.
    void enqueueCustomer(Customer* c);

    // Removes and returns the customer at the front
    Customer* dequeueCustomer();

    // Status checkers
    bool isQueueEmpty() const;
    bool isEmpty() const;
    int getQueueSize() const;

    // Returns a specific customer by their index in the line
    Customer* getCustomerAtIndex(int index) const;

    // Updates waiting time for all currently waiting customers
    void updateWaitingTimes(int currentTime);
};

// QUEUE_H