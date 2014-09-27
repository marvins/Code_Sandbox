

#  Set our binary path
BIN=bin

#  Set our source path
SRC=.

#  Set our MPI path
MPICXX=mpic++-mpich-devel-mp

#  Set our bin list
all: $(BIN)/boilerplate-mpi $(BIN)/mpi-dynamic-windows-01


#  Boilerplate Example
$(BIN)/boilerplate-mpi:$(SRC)/boilerplate-mpi.cpp
	$(MPICXX) $< -o $@

#  Dynamic Window Example 1
$(BIN)/mpi-dynamic-windows-01:$(SRC)/mpi-dynamic-windows-01.cpp
	$(MPICXX) $< -o $@

