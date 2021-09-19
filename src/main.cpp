#include "tests/CycleDeadlockTest.h"
#include "tests/NoDeadlockTest.h"
#include "tests/SimpleDeadlockTest.h"

#include <iostream>

int main() {
    CycleDeadlockTest();
    NoDeadlockTest();
    SimpleDeadlockTest();

    std::cout << "All tests passed" << std::endl;
}
