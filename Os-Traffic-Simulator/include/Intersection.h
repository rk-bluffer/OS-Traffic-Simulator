#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <queue>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include "Vehicle.h"
#include "Semaphore.h" 
#include "TrafficOS_Radio.h" // NEW: Include the radio

using namespace std;

class Intersection {
private:
    priority_queue<Vehicle> lanes[4];
    mutex mtx;
    condition_variable cv;
    Semaphore roadCapacity; 

public:
    // NEW: Public access to the Radio (So the Admin Thread can toggle buttons)
    TrafficOS_Radio systemRadio;

    Intersection(int max_cars) : roadCapacity(max_cars) {}

    void addVehicle(Vehicle v);
    void trafficControllerLoop();
};

#endif