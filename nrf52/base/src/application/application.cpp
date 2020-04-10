#include "application.h"

Application::Application()
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

}
