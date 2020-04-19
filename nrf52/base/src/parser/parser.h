#ifndef NRF52_MODULARITY_PARSER_H
#define NRF52_MODULARITY_PARSER_H

#include "general.h"
#include "executor/executor.h"
#include "nrf_cli.h"

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
class Application;

class Parser
{
public:
    Parser(Executor& executor): _executor(executor) {}

    ReturnCode parseCommand(nrf_cli_t const * p_cli, size_t argc, char **argv);
private:
    Executor& _executor;
    static const size_t MAX_CHUNK_BUFFER_SIZE = 32U;
    uint8_t _buffer[MAX_CHUNK_BUFFER_SIZE];
};

#endif //NRF52_MODULARITY_PARSER_H
