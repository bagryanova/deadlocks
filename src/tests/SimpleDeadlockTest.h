#pragma once

#include "SafeMutexFactory.h"

#include <atomic>
#include <iostream>

void SimpleDeadlockTest() {
    std::cout << "Running SimpleDeadlockTest" << std::endl;

    TSafeMutexFactory mutexFactory;
    auto m1 = mutexFactory.CreateSafeMutex();
    auto m2 = mutexFactory.CreateSafeMutex();
    std::atomic<bool> deadlockFound(false);

    std::thread t1(
        [&]() {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::lock_guard lock1(m1);
                std::lock_guard lock2(m2);
            } catch (const std::exception& e) {
                deadlockFound.store(true);
            }
        });

    std::thread t2(
        [&]() {
            try {
                std::lock_guard lock1(m2);
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                std::lock_guard lock2(m1);
            } catch (const std::exception& e) {
                deadlockFound.store(true);
            }
        });

    t1.join();
    t2.join();

    if (!deadlockFound)
        throw std::logic_error("SimpleDeadlockTest failed: deadlock was not found");

    std::cout << "SimpleDeadlockTest passed" << std::endl;
}
