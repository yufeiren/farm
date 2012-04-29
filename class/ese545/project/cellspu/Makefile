all: scc cellspu

scc: scc.c
	gcc scc.c -o scc

cellspu: cellspu.v rf.v exmemory.v controller.v top.v
	iverilog -o cellspu cellspu.v rf.v exmemory.v controller.v top.v

clean:
	rm -rf scc
