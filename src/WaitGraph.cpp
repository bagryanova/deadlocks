#include "WaitGraph.h"

void TWaitGraph::ThreadWaitingForMutex(
    const TWaitGraph::TThreadId threadId,
    const TWaitGraph::TMutexId mutexId)
{
    {
        std::unique_lock lock(Mutex);
        ThreadWaitingFor[threadId] = mutexId;
    }

    {
        std::shared_lock lock(Mutex);
        CheckDeadlocks(threadId);
    }
}

void TWaitGraph::MutexLocked(
    const TWaitGraph::TThreadId threadId,
    const TWaitGraph::TMutexId mutexId)
{
    {
        std::unique_lock lock(Mutex);
        ThreadWaitingFor.erase(threadId);
        MutexLockedBy[mutexId] = threadId;
    }

    {
        std::shared_lock lock(Mutex);
        CheckDeadlocks(threadId);
    }
}

void TWaitGraph::MutexUnlocked(
    const TWaitGraph::TThreadId threadId,
    const TWaitGraph::TMutexId mutexId)
{
    std::unique_lock lock(Mutex);
    if (MutexLockedBy.at(mutexId) == threadId)
        MutexLockedBy.erase(mutexId);
}

void TWaitGraph::CheckDeadlocks(const TWaitGraph::TThreadId startThreadId) const {
    auto currentThread = startThreadId;
    std::unordered_set<std::thread::id> threadIds;

    while (true) {
        threadIds.insert(currentThread);

        {
            const auto mutexIterator = ThreadWaitingFor.find(currentThread);
            if (mutexIterator == ThreadWaitingFor.end())
                break;

            const auto threadIterator = MutexLockedBy.find(mutexIterator->second);
            if (threadIterator == MutexLockedBy.end())
                break;

            currentThread = threadIterator->second;
        }

        if (threadIds.count(currentThread) > 0)
            throw std::runtime_error("Deadlock found");
    }
}
