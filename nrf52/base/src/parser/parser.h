#ifndef NRF52_MODULARITY_PARSER_H
#define NRF52_MODULARITY_PARSER_H

#include "general.h"

/**
 * This class is inteded to parse data command from the CLI.
 * argv[0] is processed on Nordic's API side.
 * Starting from there, next analysis steps are performed here.
 * Example command: application data 00 8 0011223344556677 43c2abef,
 * where data - command selection,
 *       00 - hex offset for placement of data chunk,
 *       8 - hex size for size of the data chunk,
 *       0011223344556677 - 8 bytes of data in hex format,
 *       43c2abef - CRC32 of the data
 */
class Parser
{
public:
    Parser() {}

    ReturnCode parseCommand(size_t argc, char **argv);
};

#endif //NRF52_MODULARITY_PARSER_H
