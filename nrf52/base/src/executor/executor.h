#ifndef NRF52_MODULARITY_EXECUTOR_H
#define NRF52_MODULARITY_EXECUTOR_H

#include "general.h"
#include "nrf_cli.h"

class Executor
{
public:
    Executor();
    void fillExecutionMemory(uint32_t offset, size_t size, uint8_t * buffer);
    void executeTestFunction(nrf_cli_t const * p_cli);

private:
    uint8_t * _ramfuncMemoryStart;
};


#endif //NRF52_MODULARITY_EXECUTOR_H
