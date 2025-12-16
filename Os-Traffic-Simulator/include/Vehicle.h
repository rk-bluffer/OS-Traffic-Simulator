#ifndef VEHICLE_H
#define VEHICLE_H

#include <chrono>

// NEW: Define the 4 Directions
enum Direction { NORTH, EAST, SOUTH, WEST };

struct Vehicle {
    int id;
    bool isEmergency; 
    Direction dir; // NEW: The direction the car is coming from
    std::chrono::time_point<std::chrono::steady_clock> arrivalTime;

    // Updated Constructor
    Vehicle(int i, bool emergency, Direction d);

    bool operator<(const Vehicle& other) const;
};

#endif