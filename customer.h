#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer {
private:
    int id;
    int arrivalTime;
    int transactionTime;
    int waitingTime;
    bool isVIP;
// queue project
//
public:
    Customer(int id = 0, int arrTime = 0, int transTime = 0, bool vip = false);
    int getArrivalTime() const;
    int getTransactionTime() const;
    int getWaitingTime() const;
    bool getIsVIP() const;
    void setWaitingTime(int time);
    void incrementWaitingTime();
};

#endif