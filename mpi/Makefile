

#  Set our binary path
BIN=bin

#  Set our source path
SRC=.

#  Set our MPI path
MPICXX=mpic++-mpich-devel-mp

#  Set the Compiler Options
CXX_OPTIONS=--std=c++11

#  Set our bin list
all: $(BIN)/boilerplate-mpi $(BIN)/mpi-dynamic-windows-01 $(BIN)/mpi-dynamic-windows-02


#  Boilerplate Example
$(BIN)/boilerplate-mpi:$(SRC)/boilerplate-mpi.cpp
	$(MPICXX) $< -o $@

#  Dynamic Window Example 1
$(BIN)/mpi-dynamic-windows-01:$(SRC)/mpi-dynamic-windows-01.cpp
	$(MPICXX) $< -o $@

#  Dynamic Window Example 2
$(BIN)/mpi-dynamic-windows-02:$(SRC)/mpi-dynamic-windows-02.cpp
	$(MPICXX) $< -o $@ $(CXX_OPTIONS)

