# openarc_sandbox
sandbox for testing openarc

## setup

```
source env.sh
```

## build and test emulation

```
mkdir build-openarc_sandbox-emu
cd !$
make -f $SANDBOX/Makefile SRC=$SANDBOX/reduction emulate

mkdir build-openarc_slidingAvg-emu
cd !$
make -f $SANDBOX/Makefile SRC=$SANDBOX/slidingAvg emulate
```

## build on fpga circuit

```
mkdir build-openarc_sandbox-fpga
cd !$
make -f $SANDBOX/Makefile SRC=$SANDBOX/reduction device

mkdir build-openarc_slidingAvg-fpga
cd !$
make -f $SANDBOX/Makefile SRC=$SANDBOX/slidingAvg device
```

## run on fpga circuit
```
1. Set up
   Create a lease with filters ["$fpga.board_model"=="385A"] and ["$node_type"=="fpga"]
   Launch an instance with image CC-CentOS7-FPGA
2. Instance environment config
   module load nallatech
   export OPENARC_ARCH=3
   export ACC_DEVICE_TYPE=acc_device_not_host
   export ACC_DEVICE_NUM=0
   export OPENARCCRT_UNIFIEDMEM=0
   export OPENARC_FPGA=ARRIA_X
3. Copy the project & run
   Append instance ssh key to pulic key list on build node
   scp -r username@fpga01.tacc.chameleoncloud.org:build_directory/cetus_output .
   cd cetus_output
   ./slidingAvg
   ./reduce_sum
   
```
