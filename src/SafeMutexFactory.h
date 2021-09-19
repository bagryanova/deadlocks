#pragma once

#include "SafeMutex.h"
#include "WaitGraph.h"

#include <memory>

class TSafeMutexFactory {
public:
    TSafeMutexFactory();

    TSafeMutex CreateSafeMutex();

    void AttachMutex(TSafeMutex& mutex);

private:
    std::shared_ptr<TWaitGraph> WaitGraph;
    std::atomic<std::size_t> NextMutexID;
};
