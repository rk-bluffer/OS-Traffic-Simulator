#include "../include/Vehicle.h"

using namespace std;

// Updated Constructor
Vehicle::Vehicle(int i, bool emergency, Direction d) 
    : id(i), isEmergency(emergency), dir(d) {
    arrivalTime = chrono::steady_clock::now();
}

bool Vehicle::operator<(const Vehicle& other) const {
    if (!isEmergency && other.isEmergency) return true;
    if (isEmergency && !other.isEmergency) return false;
    return arrivalTime > other.arrivalTime;
}