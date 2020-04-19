#include <cstring>
#include "general.h"
#include "application.h"
#include "nrf_gpio.h"
#include "nrf_cli.h"

Application::Application()
: _executor()
, _parser(_executor)
{

}

void Application::init()
{

}

void Application::cyclic()
{

}


Application application;


extern "C" 
{
    
void application_init()
{

    application.init();
}

void application_cyclic()
{
    application.cyclic();
}

int application_parse_cmd(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    ReturnCode result = application.getParser().parseCommand(p_cli, argc, argv);
    return static_cast<int>(result);
}


}
