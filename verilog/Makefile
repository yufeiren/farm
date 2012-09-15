all: hello_world

hello_world: hello_world.v
	iverilog $@.v -o $@

clean:
	rm -rf hello_world
