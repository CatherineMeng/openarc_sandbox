# openarc_sandbox
sandbox for testing openarc

## setup

```
source env.sh
```

## build and test emulation

```
mkdir build-openarc_sandbox
cd !$
make -f $SANDBOX/Makefile SRC=$SANBOX/reduction emulate
```

## build and test fpga circuit

```
mkdir build-openarc_sandbox
cd !$
make -f $SANDBOX/Makefile SRC=$SANBOX/reduction device
```
