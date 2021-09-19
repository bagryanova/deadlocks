#pragma once

#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

class TWaitGraph {
public:
    using TMutexId = std::size_t;
    using TThreadId = std::thread::id;

public:
    void ThreadWaitingForMutex(const TThreadId threadId, const TMutexId mutexId);

    void MutexLocked(const TThreadId threadId, const TMutexId mutexId);

    void MutexUnlocked(const TThreadId threadId, const TMutexId mutexId);

private:
    void CheckDeadlocks(const TThreadId startThreadId) const;

private:
    std::shared_mutex Mutex;

    std::unordered_map<TThreadId, TMutexId> ThreadWaitingFor;
    std::unordered_map<TMutexId, TThreadId> MutexLockedBy;
};
