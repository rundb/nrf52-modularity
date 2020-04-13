# NRF52 Modularity PoC

This is a toolchain for compilation of project `nrf52-modularity` for Nordic NRF52 microcontroller.

To build docker image for cross-compilation, use `docker build -t nrf52-toolchain:v0 .`

To use this image afterwards, use `docker run -it nrf52-toolchain:v0 /bin/bash`. All tools related to NRF52 development are stored in `/tools` folder. 

Please be aware that all the necessary tools are held within the image, so building it won't make SDK available in the host system. In order 
to resolve that issue and be able to reference the SDK from the host machine, you have to somehow obtain copy of the SDK on  host machine.
One of the options is to enter image and copy the SDK folder (`/tools/nRF...`) into the mounted volume.

In order to assure that minimal basic toolchain works, in the running image perform the following actions:
`cd /tools/nRF5SDK160098a08e2/examples/peripheral/blinky/pca10056/blank/armgcc/ && make`. Build process should pass without errors.

Build project script: `cd nrf52 && docker run -v $(pwd):/host:cached  nrf52-toolchain:v0 /bin/bash -c "cd /host/base/src && make" && cd ../`

Flashing is performed this way: `nrfjprog -f nrf52 --program ./nrf52/base/src/_build/nrf52840_xxaa.hex --sectorerase && nrfjprog --reset`

## Upload dump

A small Python tool has been introduce to perform upload of the test dump to the chip. One can find it by path `python/upload_dump/upload_dump.py`

Requirement: library `pyserial` (`pip3 install pyserial`)

Usage: `python3 python/upload_dump/upload_dump.py -p /path/to/tty/PORT -P path/to/binary`.

It is very raw and primitive, it's purpose just to get started with uploading the data and executing it. All further improvements can be done further.

Right after usage, Nordic DK attempts to execute the code from the test memory. If it fails, it calls `UsageFault_Handler()`. This handler blinks
LED1 on the board 10 times and then resets the board. 