#include "parser/parser.h"
#include <string.h>

ReturnCode
Parser::parseCommand(size_t argc, char **argv)
{
    if (!strcmp(argv[1], "data"))
    {
        return RES_OK;
    }
    return RES_ERROR;
}
