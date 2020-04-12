#include "executor/executor.h"
#include <string.h>

extern uint8_t ramFuncExecutionMemory[];
extern size_t ramFuncExecutionMemorySize;
extern void functionForExecution();
static const uint8_t DEFAULT_RAM_EXECUTION_MEMORY_VALUE = 0xABU;

Executor::Executor()
{
    _ramfuncMemoryStart = (uint8_t *)functionForExecution;
    memset(ramFuncExecutionMemory, DEFAULT_RAM_EXECUTION_MEMORY_VALUE, ramFuncExecutionMemorySize);
}

void Executor::fillExecutionMemory(uint32_t offset, size_t size, uint8_t * buffer)
{
    memcpy(&_ramfuncMemoryStart[offset], buffer, size);
}

void Executor::executeTestFunction()
{
    functionForExecution();
}
