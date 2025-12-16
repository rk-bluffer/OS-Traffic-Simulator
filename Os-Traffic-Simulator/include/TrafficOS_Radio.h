#ifndef TRAFFIC_OS_RADIO_H
#define TRAFFIC_OS_RADIO_H

#include <atomic>

// This struct acts as the "Operating System Control Panel"
struct TrafficOS_Radio {
    // Atomic variables are safe to read/write from multiple threads without a mutex
    std::atomic<bool> heavyRainMode{false};        // If true, slows down everything
    std::atomic<bool> emergencyBroadcast{false};   // If true, forces a system yield
};

#endif