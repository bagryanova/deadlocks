#include "SafeMutexFactory.h"

TSafeMutexFactory::TSafeMutexFactory()
    : WaitGraph(std::make_shared<TWaitGraph>())
    , NextMutexID(0)
{
}

TSafeMutex TSafeMutexFactory::CreateSafeMutex() {
    return TSafeMutex(WaitGraph, NextMutexID.fetch_add(1));
}

void TSafeMutexFactory::AttachMutex(TSafeMutex& mutex) {
    mutex.SetWaitGraphAndId(WaitGraph, NextMutexID.fetch_add(1));
}
