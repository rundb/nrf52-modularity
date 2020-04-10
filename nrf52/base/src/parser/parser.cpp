#include "parser/parser.h"
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include "nrf_log.h"

ReturnCode
Parser::parseCommand(size_t argc, char **argv)
{
    if (!strcmp(argv[1], "data"))
    {
        // application data OFFSET LEN DATA CRC
        if (argc != 6)
        {
            return RES_ERROR;
        }
//        uint8_t offset = static_cast<uint8_t>(strtol(argv[2], NULL, 16));
        uint8_t size = static_cast<uint8_t>(strtol(argv[3], NULL, 16));
        NRF_LOG_RAW_INFO("size 0x%x\n", size);
        if (size > MAX_CHUNK_BUFFER_SIZE)
        {
            return RES_ERROR;
        }
        for (int i = 0; i < size; i += 1)
        {
            char tmp[2] = {argv[4][2 * i], argv[4][2 * i + 1]};
            _buffer[i] = static_cast<uint8_t>(strtol(tmp, NULL, 16));
        }
        NRF_LOG_RAW_INFO("first 2: rx[0x%x,0x%x]\n", _buffer[0], _buffer[1]);

        return RES_OK;
    }
    return RES_ERROR;
}
