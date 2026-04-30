#ifndef CUSTOMER_H
#define CUSTOMER_H

enum Status { WAITING, SERVING, DONE };

class Customer {
private:
    int id;
    int arrivalTime;
    int transactionTime;
    int waitingTime;
    bool isVIP;
    Status status;

public:
    Customer(int id = 0, int arrTime = 0, int transTime = 0, bool vip = false);

    int getId() const;
    int getArrivalTime() const;
    int getTransactionTime() const;
    int getWaitingTime() const;
    bool getIsVIP() const;
    Status getStatus() const;

    void setWaitingTime(int time);
    void incrementWaitingTime();
    void setStatus(Status s);
};

#endif
