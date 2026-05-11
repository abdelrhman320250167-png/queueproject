#include "Queue.h"

Node::Node(Customer* c) : customer(c), next(nullptr) {}

// Private helper method to append a node directly (used in Copy/Assignment)
void Queue::appendNode(Customer* c) {
    Node* newNode = new Node(c);
    if (!front) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

// Private helper method to clear the linked list
void Queue::clear() {
    while (front != nullptr) {
        Node* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
    count = 0;
}

// Default Constructor initializes an empty queue
Queue::Queue() : front(nullptr), rear(nullptr), count(0) {}

// Copy Constructor for deep copy of the queue structure
Queue::Queue(const Queue& other) : front(nullptr), rear(nullptr), count(0) {
    Node* current = other.front;
    while (current != nullptr) {
        appendNode(current->customer);
        current = current->next;
    }
}

// Assignment Operator
Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        clear(); // Clear existing nodes
        Node* current = other.front;
        while (current != nullptr) {
            appendNode(current->customer);
            current = current->next;
        }
    }
    return *this;
}

// Destructor to free dynamically allocated nodes
Queue::~Queue() {
    clear();
}

// Adds a customer to the queue. VIPs are inserted at the front.
void Queue::enqueueCustomer(Customer* c) {
    if (!c) return;

    Node* newNode = new Node(c);

    if (isEmpty()) {
        front = rear = newNode;
    } else if (c->getIsVIP()) {
        // VIP goes to the front of the line
        newNode->next = front;
        front = newNode;
    } else {
        // Normal customer goes to the rear
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

// Removes and returns the customer at the front
Customer* Queue::dequeueCustomer() {
    if (isEmpty()) return nullptr;

    Node* temp = front;
    Customer* c = temp->customer;
    
    front = front->next;
    if (front == nullptr) {
        rear = nullptr; // If queue becomes empty, rear should also be null
    }

    delete temp;
    count--;
    return c;
}

// Status checkers
bool Queue::isQueueEmpty() const { return count == 0; }
bool Queue::isEmpty() const { return count == 0; }
int Queue::getQueueSize() const { return count; }

// Returns a specific customer by their index in the line
Customer* Queue::getCustomerAtIndex(int index) const {
    if (index < 0 || index >= count) return nullptr;
    
    Node* current = front;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->customer;
}

// Updates waiting time for all currently waiting customers
void Queue::updateWaitingTimes(int currentTime) {
    Node* current = front;
    while (current != nullptr) {
        if (current->customer) {
            current->customer->calculateWaitingTime(currentTime);
        }
        current = current->next;
    }
}