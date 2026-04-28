#ifndef SERVER_H
#define SERVER_H
#include "Customer.h"

class Server {
private:
    Customer currentCustomer;
    bool busy;
    int remainingTime;

public:
    Server();
    bool isFree() const;
    void assignCustomer(Customer c);
    void updateTime(); 
    void finishService();
    Customer getCurrentCustomer() const;
};

#endif