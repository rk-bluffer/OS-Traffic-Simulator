#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count; // The number of available slots

public:
    // Initialize with a specific limit (e.g., 5 slots)
    Semaphore(int start_count) : count(start_count) {}

    // "Wait" / "P" operation: Decrease count, block if 0
    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0) {
            cv.wait(lock); // Wait here if road is full
        }
        count--; // Take a slot
    }

    // "Signal" / "V" operation: Increase count, wake up waiting threads
    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        count++; // Return a slot
        cv.notify_one(); // Wake up a blocked generator
    }
};

#endif