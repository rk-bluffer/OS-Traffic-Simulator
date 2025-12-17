#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <queue>
#include <vector>
#include <algorithm> // Needed for finding/removing cars
#include <mutex>
#include <condition_variable>
#include "Vehicle.h"
#include "Semaphore.h" 
#include "TrafficOS_Radio.h"

using namespace std;

class Intersection {
private:
    priority_queue<Vehicle> lanes[4];
    mutex mtx;
    condition_variable cv;
    Semaphore roadCapacity; 

public:
    TrafficOS_Radio systemRadio;
    
    // --- VISUALIZATION DATA (The missing part causing your errors) ---
    // A separate list just for the graphics engine to read
    vector<Vehicle> displayList; 
    // Tracks which light is currently Green (-1 = All Red)
    int currentGreenLaneIndex = -1; 

    Intersection(int max_cars) : roadCapacity(max_cars) {}

    void addVehicle(Vehicle v);
    void trafficControllerLoop();
    
    // Thread-Safe Helper for Visualizer
    vector<Vehicle> getDisplayState(int& greenLane) {
        unique_lock<mutex> lock(mtx);
        greenLane = currentGreenLaneIndex;
        return displayList; // Return a copy of the cars
    }
};

#endif