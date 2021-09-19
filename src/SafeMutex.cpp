#include "SafeMutex.h"

void TSafeMutex::lock() {
    if (WaitGraph)
        WaitGraph->ThreadWaitingForMutex(std::this_thread::get_id(), Id);

    Mutex.lock();

    if (WaitGraph)
        WaitGraph->MutexLocked(std::this_thread::get_id(), Id);
}

bool TSafeMutex::try_lock() {
    if (Mutex.try_lock()) {
        if (WaitGraph)
            WaitGraph->MutexLocked(std::this_thread::get_id(), Id);
        return true;
    }
    return false;
}

void TSafeMutex::unlock() {
    if (WaitGraph)
        WaitGraph->MutexUnlocked(std::this_thread::get_id(), Id);
    Mutex.unlock();
}

TSafeMutex::TSafeMutex(
    std::shared_ptr<TWaitGraph> waitGraph,
    const TWaitGraph::TMutexId id)
    : WaitGraph(waitGraph)
    , Id(id)
{
}

void TSafeMutex::SetWaitGraphAndId(
    std::shared_ptr<TWaitGraph> waitGraph,
    const TWaitGraph::TMutexId id)
{
    WaitGraph = waitGraph;
    Id = id;
}
