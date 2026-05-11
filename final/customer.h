#pragma once

// Customer.h
// Defines the Customer class for the queue simulation.

// Customer service states
enum class Status {
    WAITING,
    SERVING,
    DONE
};

class Customer {
private:
    int id;            // Unique Identifier
    int arrivalTime;   // Time of arrival
    int serviceTime;   // Required service duration
    int waitingTime;   // Time spent in queue
    bool isVIP;        // VIP status flag
    Status status;     // Current service state

public:
    // Constructor with default values
    Customer(int id_ = 0, int arrTime = 0, int serviceTime_ = 0, bool vip = false);

    // Getters
    int getId() const;
    int getArrivalTime() const;
    int getServiceTime() const;
    int getWaitingTime() const;
    bool getIsVIP() const;
    Status getStatus() const;

    // Setters
    void setWaitingTime(int time);
    void setStatus(Status s);

    // Updates waiting time based on current time (only if still waiting)
    void calculateWaitingTime(int currentTime);
};