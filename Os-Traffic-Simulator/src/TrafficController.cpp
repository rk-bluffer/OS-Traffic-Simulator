#include "../include/Intersection.h"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

// Helper to get text name of direction
string getDirName(Direction d) {
    switch(d) {
        case NORTH: return "NORTH";
        case EAST: return "EAST";
        case SOUTH: return "SOUTH";
        case WEST: return "WEST";
        default: return "UNKNOWN";
    }
}

// --- PRODUCER ---
void Intersection::addVehicle(Vehicle v) {
    roadCapacity.acquire(); 

    {
        unique_lock<mutex> lock(mtx);
        // Add to the specific lane queue based on direction
        lanes[v.dir].push(v);
        
        cout << "[SENSOR] " << (v.isEmergency ? "AMBULANCE" : "Vehicle") 
             << " #" << v.id << " arrived at " << getDirName(v.dir) << " Lane." << endl;
    }
    cv.notify_all(); // Wake up controller
}

// --- CONSUMER (Round Robin Scheduler) ---
void Intersection::trafficControllerLoop() {
    int currentLaneIndex = 0; // Start with NORTH (0)

    while (true) {
        // 1. Pick the current direction (Round Robin)
        Direction currentDir = static_cast<Direction>(currentLaneIndex);
        bool carFound = false;
        Vehicle current(0, false, NORTH);

        // 2. CRITICAL SECTION (Check the queue)
        {
            unique_lock<mutex> lock(mtx);
            
            // Check if there is a car in the CURRENT lane
            if (!lanes[currentLaneIndex].empty()) {
                current = lanes[currentLaneIndex].top();
                lanes[currentLaneIndex].pop();
                carFound = true;
            } 
            // EMERGENCY CHECK: Look at ALL other lanes for an Ambulance
            else {
                // (Optional Priority Inversion Logic could go here)
            }
        }

        // 3. PROCESS THE CAR (Outside Lock)
        if (carFound) {
            roadCapacity.release(); // Free up space

            cout << "\n[LIGHT] " << getDirName(currentDir) << " LIGHT: GREEN -> Vehicle #" << current.id << " crossing." << endl;
            
            if (current.isEmergency) {
                cout << "!!! PRIORITY PASSAGE !!!" << endl;
                this_thread::sleep_for(chrono::milliseconds(500)); 
            } else {
                this_thread::sleep_for(chrono::seconds(1)); 
            }
        } else {
            // Lane is empty, switch immediately to next lane (No sleep)
            // cout << "[LIGHT] " << getDirName(currentDir) << " Lane Empty. Switching..." << endl;
        }

        // 4. ROUND ROBIN: Move to next lane (0 -> 1 -> 2 -> 3 -> 0)
        currentLaneIndex = (currentLaneIndex + 1) % 4;
        
        // Small delay to prevent CPU burning if all empty
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}