#include "general.h"
#include "application.h"


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

int application_parse_cmd(size_t argc, char **argv)
{
    ReturnCode result = application.getParser().parseCommand(argc, argv);
    return static_cast<int>(result);
}


}
