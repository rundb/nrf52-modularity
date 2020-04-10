FROM ubuntu:xenial
MAINTAINER r_und_b

RUN   apt-get update \
      && apt-get install -y \
        build-essential \
        wget \
        make \
        git \
        vim \
        curl \
        unzip

RUN mkdir /tools && cd /tools

# TODO: replace these names with variable
ARG GCC_NAME=gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux
ARG GCC_FOLDER_NAME=gcc-arm-none-eabi-9-2019-q4-major
ARG SDK_NAME=nRF5SDK160098a08e2
ARG SOFT_DEVICE_NAME=s140nrf52701

RUN     wget -P /tools -O /tools/${GCC_NAME}.tar.bz2  "https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2?revision=6e63531f-8cb1-40b9-bbfc-8a57cdfc01b4&la=en&hash=F761343D43A0587E8AC0925B723C04DBFB848339"

RUN     wget -P /tools  "https://www.nordicsemi.com/-/media/Software-and-other-downloads/SDKs/nRF5/Binaries/nRF5SDK160098a08e2.zip"

RUN     wget -P /tools "https://www.nordicsemi.com/-/media/Software-and-other-downloads/SoftDevices/S140/s140nrf52701.zip"

RUN mkdir /tools/cmake && wget -P /tools/cmake "https://github.com/Kitware/CMake/releases/download/v3.16.2/cmake-3.16.2-Linux-x86_64.sh"

RUN cd /tools && tar xjfv gcc-arm* && rm gcc-*.tar.bz2

RUN cd /tools && mkdir ${SDK_NAME} && unzip nRF5SDK*.zip -d ${SDK_NAME} && rm nRF5*.zip

RUN cd /tools && mkdir ${SOFT_DEVICE_NAME} && unzip s1*.zip -d ${SOFT_DEVICE_NAME} && rm s1*.zip

RUN cd /tools/cmake && chmod +x cmake*.sh && ./cmake-3.16.2-Linux-x86_64.sh --skip-license && export PATH="$PATH:/tools/bin"

ARG TOOLCHAIN_DESCRIPTOR=/tools/nRF5SDK160098a08e2/components/toolchain/gcc/Makefile.posix

RUN rm ${TOOLCHAIN_DESCRIPTOR} && touch ${TOOLCHAIN_DESCRIPTOR} && echo 'GNU_INSTALL_ROOT ?= /tools/gcc-arm-none-eabi-9-2019-q4-major/bin/\nGNU_VERSION ?= 9.2.1\nGNU_PREFIX ?= arm-none-eabi\n' > ${TOOLCHAIN_DESCRIPTOR}

ENV PATH="${PATH}:/tools/${GCC_FOLDER_NAME}/bin"
ENV PATH="${PATH}:/tools/cmake/bin"


#/tools/gcc-arm-none-eabi-9-2019-q4-major/bin
#/tools/s140nrf52701/s140_nrf52_7.0.1_softdevice.hex
