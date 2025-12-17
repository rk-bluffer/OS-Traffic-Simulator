#include "../include/Intersection.h"
#include <thread>
#include <chrono>
#include <string>
#include<iostream>
using namespace std;

string getDirName(Direction d) {
    switch(d) {
        case NORTH: return "NORTH"; case EAST: return "EAST";
        case SOUTH: return "SOUTH"; case WEST: return "WEST";
        default: return "UNKNOWN";
    }
}

void Intersection::addVehicle(Vehicle v) {
    roadCapacity.acquire(); 

    {
        unique_lock<mutex> lock(mtx);
        lanes[v.dir].push(v);
        
        // VISUALIZER UPDATE: Add car to display list
        displayList.push_back(v); 

        cout << "[SENSOR] " << (v.isEmergency ? "AMBULANCE" : "Vehicle") 
             << " #" << v.id << " arrived at " << getDirName(v.dir) << " Lane." << endl;
    }
    cv.notify_all(); 
}

void Intersection::trafficControllerLoop() {
    int currentLaneIndex = 0; 

    while (true) {
        unique_lock<mutex> lock(mtx);
        currentGreenLaneIndex = currentLaneIndex; // Update Graphics state

        // Rain Mode Handling
        if (systemRadio.heavyRainMode) {
             this_thread::sleep_for(chrono::milliseconds(500)); 
        }

        // Global Sleep (Empty Road Interrupt)
        while (lanes[0].empty() && lanes[1].empty() && lanes[2].empty() && lanes[3].empty()) {
            currentGreenLaneIndex = -1; // Turn lights Red visually
            cv.wait(lock);
            currentGreenLaneIndex = currentLaneIndex;
        }

        // Round Robin Skip
        if (lanes[currentLaneIndex].empty()) {
            currentLaneIndex = (currentLaneIndex + 1) % 4;
            continue; 
        }

        // Process Vehicle
        Vehicle current = lanes[currentLaneIndex].top();
        lanes[currentLaneIndex].pop();

        // VISUALIZER UPDATE: Remove car from display list
        for(auto it = displayList.begin(); it != displayList.end(); ++it) {
            if(it->id == current.id) {
                displayList.erase(it);
                break;
            }
        }
        
        lock.unlock();
        roadCapacity.release(); 

        cout << "\n[LIGHT] " << getDirName(static_cast<Direction>(currentLaneIndex)) 
             << " LIGHT: GREEN -> Vehicle #" << current.id << " crossing." << endl;
        
        int crossingTime = current.isEmergency ? 500 : 1000;
        if (systemRadio.heavyRainMode) crossingTime *= 2; 

        this_thread::sleep_for(chrono::milliseconds(crossingTime));

        currentLaneIndex = (currentLaneIndex + 1) % 4;
    }
}