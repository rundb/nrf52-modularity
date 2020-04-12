#include <cstring>
#include "general.h"
#include "application.h"
#include "nrf_gpio.h"

#define CODE_MEMORY_SIZE        (0x400)

__attribute__ ((aligned (32)))
static volatile uint8_t codeExecMemoryArea[CODE_MEMORY_SIZE];
char led_code[] = {0x80, 0xb4, 0x83, 0xb0, 0x00, 0xaf, 0x16, 0x4b, 0x1b, 0x68, 0x03, 0xf1, 0xa0, 0x53, 0x03, 0xf5,
                   0xe0, 0x73, 0x9b, 0x00, 0x1a, 0x46, 0x01, 0x23, 0x13, 0x60, 0x12, 0x4a, 0x10, 0x4b, 0x1b, 0x68,
                   0x01, 0x21, 0x01, 0xfa, 0x03, 0xf3, 0x13, 0x60, 0x00, 0x23, 0x7b, 0x60, 0x02, 0xe0, 0x7b, 0x68,
                   0x01, 0x33, 0x7b, 0x60, 0x7b, 0x68, 0x0c, 0x4a, 0x93, 0x42, 0xf8, 0xdd, 0x0b, 0x4a, 0x08, 0x4b,
                   0x1b, 0x68, 0x01, 0x21, 0x01, 0xfa, 0x03, 0xf3, 0x13, 0x60, 0x00, 0x23, 0x3b, 0x60, 0x02, 0xe0,
                   0x3b, 0x68, 0x01, 0x33, 0x3b, 0x60, 0x3b, 0x68, 0x03, 0x4a, 0x93, 0x42, 0xf8, 0xdd, 0xdc, 0xe7,
                   0x00, 0x00, 0x00, 0x00, 0x08, 0x05, 0x00, 0x50, 0x3f, 0x42, 0x0f, 0x00, 0x0c, 0x05, 0x00, 0x50};

static const uint32_t handMadeVeneer[2] __attribute__((section(".handmade_veneer")))  __attribute__ ((__used__)) = {0xf85ff000, 0x20000001};
extern uint8_t ramFuncExecutionMemory[];

Application::Application()
: _parser()
{

}

void Application::init()
{

}

void Application::cyclic()
{

}

Application application;

typedef void (*FooWithoutArgsType)();
extern void functionForExecution();


FooWithoutArgsType foo;
void executeTargetFunction()
{
    foo();
}

extern "C" 
{
    
void application_init()
{
    for (int i = 0; i < 1024; ++i)
    {
        ramFuncExecutionMemory[i] = 0;
    }

    application.init();

//    uint32_t jumpAddressValuePtr = (uint32_t)&jumpAddressValue;
    for (int i = 0; i < 0x4; ++i)
    {
        codeExecMemoryArea[4*i] = ((uint8_t *)functionForExecution)[4*i + 3] + (handMadeVeneer[1] & 0x01);
        codeExecMemoryArea[4*i+1] = ((uint8_t *)functionForExecution)[4*i + 0];
        codeExecMemoryArea[4*i+2] = ((uint8_t *)functionForExecution)[4*i + 1];
        codeExecMemoryArea[4*i+3] = ((uint8_t *)functionForExecution)[4*i + 2];
        codeExecMemoryArea[4*i] -= (handMadeVeneer[1] & 0x01);
    }
    uint32_t codeMemoryPtr/*dataMemoryPtr*/ = (uint32_t) codeExecMemoryArea;
    //uint32_t ptrOffset = dataMemoryPtr - 0x20000000;
    //uint32_t codeMemoryPtr = 0x800000 + ptrOffset;
    foo = (FooWithoutArgsType) codeMemoryPtr;


    functionForExecution();
}

void application_cyclic()
{
    application.cyclic();
}

int application_parse_cmd(size_t argc, char **argv)
{
    ReturnCode result = application.getParser().parseCommand(argc, argv);
    return static_cast<int>(result);
}


}
