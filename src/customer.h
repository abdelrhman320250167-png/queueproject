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
    Customer(int id_ = 0, int arrTime = 0, int serviceTime_ = 0, bool vip = false)
        : id(id_),
          arrivalTime(arrTime),
          serviceTime(serviceTime_),
          waitingTime(0),
          isVIP(vip),
          status(Status::WAITING) {}

    // Getters
    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getServiceTime() const { return serviceTime; }
    int getWaitingTime() const { return waitingTime; }
    bool getIsVIP() const { return isVIP; }
    Status getStatus() const { return status; }

    // Setters
    void setWaitingTime(int time) { waitingTime = time; }
    void setStatus(Status s) { status = s; }

    // Updates waiting time based on current time (only if still waiting)
    void calculateWaitingTime(int currentTime) {
        if (status == Status::WAITING) {
            waitingTime = currentTime - arrivalTime;
        }
    }
};