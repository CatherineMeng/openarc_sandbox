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
make -f $SANDBOX/Makefile SRC=$SANDBOX/slidingAvg device
```

## build and test fpga circuit

```
mkdir build-openarc_sandbox-fpga
cd !$
make -f $SANDBOX/Makefile SRC=$SANDBOX/reduction device
make -f $SANDBOX/Makefile SRC=$SANDBOX/slidingAvg device
```
