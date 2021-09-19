#pragma once

#include "WaitGraph.h"

#include <cstdint>
#include <memory>
#include <mutex>

class TSafeMutex {
    friend class TSafeMutexFactory;
public:
    TSafeMutex() = default;

    void lock();

    bool try_lock();

    void unlock();

private:
    TSafeMutex(
        std::shared_ptr<TWaitGraph> waitGraph,
        const TWaitGraph::TMutexId id);

    void SetWaitGraphAndId(
        std::shared_ptr<TWaitGraph> waitGraph,
        const TWaitGraph::TMutexId id);

private:
    std::mutex Mutex;
    TWaitGraph::TMutexId Id;

    std::shared_ptr<TWaitGraph> WaitGraph;
};
