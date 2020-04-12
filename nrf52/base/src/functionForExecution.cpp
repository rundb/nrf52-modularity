#include "general.h"

#define RAMFUNC_EXECUTION_MEMORY_SIZE       (1024)

uint8_t __attribute__((used, section(".myRamfuncArea"), aligned (32)))
ramFuncExecutionMemory[RAMFUNC_EXECUTION_MEMORY_SIZE];
volatile const size_t __attribute__((used)) ramFuncExecutionMemorySize =  RAMFUNC_EXECUTION_MEMORY_SIZE;

__attribute__((used, long_call, section(".myRamfunc")))  void functionForExecution()
{
    __asm__("nop;");
}