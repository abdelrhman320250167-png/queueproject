#ifndef SERVER_H
#define SERVER_H

#include "Customer.h"

class Server {
private:
    Customer* currentCustomer;
    bool busy;
    int remainingTime;
    int servedCount;

public:
    Server();

    bool isFree() const;
    bool isBusy() const;

    void assignCustomer(Customer* c);
    void updateTime();
    void finishService();

    Customer* getCurrentCustomer() const;
    
    int getRemainingTime() const; 
    int getTotalServedCount() const; 
    void printStatus() const;
};

#endif
