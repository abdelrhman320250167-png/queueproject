#include "simulation.h"
#include "server.h"
#include "queue.h"
#include "customer.h"
#include <vector>

// بنعرف متغير للوقت عشان نستخدمه في الحركة الوهمية
static int internalTime = 0;

Customer::Customer(int id, int arrTime, int transTime, bool vip) {}
int Customer::getId() const { 
    // حركة صايعة: هنطلع ID مختلف لكل دايرة بناءً على مكانها في الميموري
    return ((long long)this / 40) % 900 + 100; 
} 
bool Customer::getIsVIP() const { 
    // نصهم VIP ونصهم عادي عشان نشوف الألوان (أصفر وتوركواز)
    return ((long long)this / 100) % 2 == 0; 
}
int Customer::getArrivalTime() const { return 0; }
int Customer::getTransactionTime() const { return 0; }
int Customer::getWaitingTime() const { return 0; }
Status Customer::getStatus() const { return WAITING; }
void Customer::setWaitingTime(int time) {}
void Customer::calculateWaitingTime(int currentTime) {}
void Customer::setStatus(Status s) {}
void Customer::printInfo() const {}

Server::Server() {}
bool Server::isFree() const { 
    // السيرفر هيفتح ويقفل كل 5 ثواني عشان تشوف اللون الأحمر والأخضر
    return (internalTime / 5) % 2 == 0; 
} 
bool Server::isBusy() const { return !isFree(); }
void Server::assignCustomer(Customer* c) {}
void Server::updateTime() {}
void Server::finishService() {}
Customer* Server::getCurrentCustomer() const { 
    static Customer dummy(99,0,0,false);
    return isFree() ? nullptr : &dummy; 
}
int Server::getRemainingTime() const { return 0; }
int Server::getTotalServedCount() const { return internalTime * 2; }
void Server::printStatus() const {}

Queue::Queue() {}
Queue::~Queue() {}
void Queue::enqueueCustomer(Customer* c) {}
Customer* Queue::dequeueCustomer() { return nullptr; }
bool Queue::isEmpty() const { return false; }
void Queue::updateWaitingTimes(int currentTime) {}
int Queue::getQueueSize() const { return 5; }

static Customer d1(101,0,0,false), d2(102,0,0,true), d3(103,0,0,false), d4(104,0,0,false), d5(105,0,0,true);
Customer* Queue::getCustomerAt(int index) const {
    Customer* dummy[] = {&d1, &d2, &d3, &d4, &d5};
    return dummy[index % 5];
}

Simulation::Simulation(int numServers, int maxSimulationTime) {
    internalTime = 0;
    this->servers.resize(numServers);
}
void Simulation::runTick() { internalTime++; }
int Simulation::getCurrentTime() const { return internalTime; }
const std::vector<Server>& Simulation::getServers() const { return servers; }
const Queue& Simulation::getQueue() const { return q; }
bool Simulation::getIsRunning() const { return true; }
const std::vector<int>& Simulation::getQueueHistory() const { 
    static std::vector<int> h = {2, 4, 3, 6, 4, 8, 5, 9, 7, 5, 8, 6, 4, 7};
    return h; 
}