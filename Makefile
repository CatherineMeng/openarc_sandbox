SRC?=./
CC=gcc
CXX=g++
arc=openarc
arcFlags=-assumeNoAliasingAmongKernelArgs
cflags=-O2 -std=c99
cppflags=-O2
target=bin
board=p385a_sch_ax115
arcflags=-D_OPENACC=201306 -D OPENARC_ARCH=3 -I/home/cwsmith/develop/OpenARC/openarcrt
arclibs=-L/home/cwsmith/develop/OpenARC/openarcrt -L/opt/altera_pro/16.0.2/hld/board/nalla_pcie/linux64/lib -L/opt/altera_pro/16.0.2/hld/host/linux64/lib -lalteracl -lacl_emulator_kernel_rt  -lalterahalmmd -lnalla_pcie_mmd -lelf -lrt -ldl -lopenaccrt_opencl -lomphelper

include $(SRC)/Makefile

makedirectories:
	mkdir -p $(target)

serial: $(SRC)/$(source).c
	$(CC) $(cflags) -o $(source)_serial $^

test_serial:
	./$(source)_serial

openarc: $(SRC)/$(source).c
	$(arc) $(arcFlags) $^

kernel_emu: cetus_output/openarc_kernel.cl
	aoc -march=emulator --board $(board) $^ -o cetus_output/openarc_kernel.aocx

kernel_device: cetus_output/openarc_kernel.cl
	aoc --board $(board) $^ -o cetus_output/openarc_kernel.aocx

cpp_host: cetus_output/$(source).cpp
	$(CXX) $(cppflags) $(arcflags) -I ../ -o cetus_output/$(source) $^ $(arclibs)

test_emu:
	cd cetus_output; CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1 ./$(source)

base: makedirectories serial test_serial openarc

emulate: base kernel_emu cpp_host test_emu

device: base kernel_device cpp_host

