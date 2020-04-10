#include "parser/parser.h"

class Application
{
public:
    Application();

    void init();
    void cyclic();
    Parser& getParser() {return _parser;}
private:
    Parser _parser;
};
