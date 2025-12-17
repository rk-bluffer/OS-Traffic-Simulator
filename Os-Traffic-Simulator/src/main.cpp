#include <iostream>
#include <thread>
#include <vector>
#include <random> 
#include "../include/Intersection.h"
#include "../include/Vehicle.h"

using namespace std;

// The Global Shared Intersection Object
Intersection smartIntersection(10); 

// Forward Declaration: Tells main that this function exists in Visualizer.cpp
void runVisualizer(Intersection* intersection);

// --- GENERATOR THREAD ---
void vehicleGenerator() {
    int carID = 1;
    while (true) {
        bool isEmergency = (rand() % 20 == 0); 
        Direction dir = static_cast<Direction>(rand() % 4);
        Vehicle v(carID++, isEmergency, dir);
        smartIntersection.addVehicle(v);
        this_thread::sleep_for(chrono::milliseconds(1000 + (rand() % 2000)));
    }
}

// --- ADMIN THREAD ---
void systemAdmin() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(15));
        bool currentState = smartIntersection.systemRadio.heavyRainMode;
        smartIntersection.systemRadio.heavyRainMode = !currentState;
        if (!currentState) cout << "\n*** [AI] RAIN MODE ON ***" << endl;
        else cout << "\n*** [AI] RAIN MODE OFF ***" << endl;
    }
}

int main() {
    cout << "=== SMART CITY OS SIMULATOR: GRAPHICAL MODE ===" << endl;
    
    // 1. Launch Logic Threads
    // We create them, but we DO NOT call .join() on them immediately.
    thread controllerThread(&Intersection::trafficControllerLoop, &smartIntersection);
    thread generatorThread(vehicleGenerator);
    thread adminThread(systemAdmin);

    // 2. Detach them so they run in the background independently
    controllerThread.detach();
    generatorThread.detach();
    adminThread.detach();

    // 3. Run Visualizer in the MAIN thread
    // This function acts as the "Main Loop" now. It will block here until the window closes.
    runVisualizer(&smartIntersection);
    
    // 4. When the window closes, we kill the program
    cout << "Window closed. Exiting simulation." << endl;
    exit(0); 

    return 0;
}