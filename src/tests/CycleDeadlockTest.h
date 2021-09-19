#pragma once

#include "SafeMutexFactory.h"

#include <atomic>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void CycleDeadlockTest() {
    std::cout << "Running CycleDeadlockTest" << std::endl;

    const std::size_t mutexCount = 10;
    TSafeMutexFactory mutexFactory;

    std::vector<TSafeMutex> mutexes(mutexCount);
    for (auto& mutex : mutexes)
        mutexFactory.AttachMutex(mutex);

    std::atomic<bool> deadlockFound(false);

    auto threadLambda =
        [&](const std::size_t mutex1, const std::size_t mutex2) {
            try {
                std::lock_guard lock1(mutexes[mutex1]);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::lock_guard lock2(mutexes[mutex2]);
            } catch(const std::exception& e) {
                deadlockFound.store(true);
            }
        };

    std::vector<std::future<void>> futures;
    for (std::size_t i = 0; i < mutexCount; i++) {
        futures.emplace_back(std::async(threadLambda, i, (i + 1) % mutexes.size()));
    }

    for (std::size_t i = 0; i < mutexCount; i++) {
        futures[i].get();
    }

    if (!deadlockFound)
        throw std::logic_error("CycleDeadlockTest failed: deadlock was not found");

    std::cout << "CycleDeadlockTest passed" << std::endl;
}
