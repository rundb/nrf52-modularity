#include "executor/executor.h"
#include "nrf_log.h"
#include <string.h>
#include "nrf_delay.h"

// I know, this is ugly, but I didn't find a proper way to print info before a reset

#ifdef NRF_LOG_LEVEL
#undef NRF_LOG_LEVEL
#endif 
#define NRF_LOG_LEVEL   4

extern "C" void normalRebootRequest();

extern uint8_t ramFuncExecutionMemory[];
extern size_t ramFuncExecutionMemorySize;
extern void functionForExecution();
static const uint8_t DEFAULT_RAM_EXECUTION_MEMORY_VALUE = 0xABU;

Executor::Executor()
{
    _ramfuncMemoryStart = (uint8_t *)functionForExecution - 1;
    memset(ramFuncExecutionMemory, DEFAULT_RAM_EXECUTION_MEMORY_VALUE, ramFuncExecutionMemorySize);
}

void Executor::fillExecutionMemory(uint32_t offset, size_t size, uint8_t * buffer)
{
    // memcpy(&_ramfuncMemoryStart[offset], buffer, size);
    for (size_t i = 0; i < size; i += 4)
    {
        _ramfuncMemoryStart[offset + i + 0] = buffer[i + 1];
        _ramfuncMemoryStart[offset + i + 1] = buffer[i + 0];
        _ramfuncMemoryStart[offset + i + 2] = buffer[i + 3];
        _ramfuncMemoryStart[offset + i + 3] = buffer[i + 2];
    }
}

void Executor::executeTestFunction(nrf_cli_t const * p_cli)
{
    functionForExecution();
    normalRebootRequest();
}

#ifdef NRF_LOG_LEVEL
#undef NRF_LOG_LEVEL
#endif 
