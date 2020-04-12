#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "parser/parser.h"
#include "executor/executor.h"

class Application
{
public:
    Application();

    void init();
    void cyclic();
    Parser& getParser() {return _parser;}
    Executor& getExecutor() {return _executor;}
private:
    Executor _executor;
    Parser _parser;
};

#endif //__APPLICATION_H__