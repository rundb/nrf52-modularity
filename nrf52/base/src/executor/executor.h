#ifndef NRF52_MODULARITY_EXECUTOR_H
#define NRF52_MODULARITY_EXECUTOR_H

#include "general.h"

class Executor
{
public:
    Executor();
    void fillExecutionMemory(uint32_t offset, size_t size, uint8_t * buffer);
    void executeTestFunction();

private:
    uint8_t * _ramfuncMemoryStart;
};


#endif //NRF52_MODULARITY_EXECUTOR_H
