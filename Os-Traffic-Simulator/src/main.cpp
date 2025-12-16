#include <iostream>
#include <thread>
#include <vector>
#include <random> 
#include "../include/Intersection.h"
#include "../include/Vehicle.h"

using namespace std;

Intersection smartIntersection(10); 

void vehicleGenerator() {
    int carID = 1;
    while (true) {
        bool isEmergency = (rand() % 10 == 0); 
        Direction dir = static_cast<Direction>(rand() % 4);
        Vehicle v(carID++, isEmergency, dir);
        smartIntersection.addVehicle(v);
        this_thread::sleep_for(chrono::milliseconds(500 + (rand() % 1000)));
    }
}

// --- NEW: SYSTEM ADMIN THREAD ---
// This acts as the "Operating System Kernel" or "AI"
// It randomly toggles system states (Signals)
void systemAdmin() {
    while (true) {
        // Wait 10 seconds
        this_thread::sleep_for(chrono::seconds(10));

        // Toggle Rain Mode
        bool currentState = smartIntersection.systemRadio.heavyRainMode;
        smartIntersection.systemRadio.heavyRainMode = !currentState;

        if (!currentState) {
            cout << "\n*** [AI ADMIN] ACTIVATING RAIN MODE (SYSTEM SLOWDOWN) ***\n" << endl;
        } else {
            cout << "\n*** [AI ADMIN] DEACTIVATING RAIN MODE (SYSTEM NORMAL) ***\n" << endl;
        }
    }
}

int main() {
    cout << "=== OS TRAFFIC SIMULATOR: DAY 5 (AI SIGNALS) ===" << endl;
    
    thread controllerThread(&Intersection::trafficControllerLoop, &smartIntersection);
    thread generatorThread(vehicleGenerator);
    
    // Launch the Admin Thread
    thread adminThread(systemAdmin);

    controllerThread.join();
    generatorThread.join();
    adminThread.join();

    return 0;
}