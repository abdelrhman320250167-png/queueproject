#include "Queue.h"

Queue::Queue() {
    front = nullptr;
    rear = nullptr;
    size = 0;
}

Queue::~Queue() {
    while (!isEmpty()) {
        dequeueCustomer();
    }
}

void Queue::enqueueCustomer(Customer* c) {
    Node* newNode = new Node(c);
    
    if (isEmpty()) {
        front = rear = newNode;
    } 
    else if (c->getIsVIP()) {
        newNode->next = front;
        front = newNode;
    } 
    else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

Customer* Queue::dequeueCustomer() {
    if (isEmpty()) return nullptr;

    Node* temp = front;
    Customer* cust = temp->data;
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    size--;
    return cust;
}

bool Queue::isEmpty() const {
    return front == nullptr;
}

void Queue::updateWaitingTimes(int currentTime) {
    Node* temp = front;
    while (temp != nullptr) {
        temp->data->calculateWaitingTime(currentTime);
        temp = temp->next;
    }
}

int Queue::getQueueSize() const {
    return size;
}

Customer* Queue::getCustomerAt(int index) const {
    if (index < 0 || index >= size) return nullptr;
    
    Node* temp = front;
    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }
    return temp->data;
}
