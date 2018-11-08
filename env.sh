#!/bin/bash -ex

#setup the aoc compiler
module load nallatech

#setup access to newer GCC and Java development environment
source /home/cwsmith/software/spack_install/linux-centos7-x86_64/gcc-centos_7.2/lmod-7.8-rfoosrn6l7ttojvsumrgowujbwlkqnl4/lmod/lmod/init/bash
module use /home/cwsmith/software/spack_install/lmod/linux-centos7-x86_64/Core

module load gcc
module load jdk/1.8.0_181-b13-o2xaneg

export SANDBOX=$PWD

export OPENARC_ARCH=3
export ACC_DEVICE_TYPE=acc_device_not_host
export ACC_DEVICE_NUM=0
export OPENARCCRT_UNIFIEDMEM=0
export PATH=~cwsmith/develop/OpenARC/bin/:~cwsmith/develop/OpenARC/openarcrt/:$PATH
export OPENARC_FPGA=ARRIA_X
