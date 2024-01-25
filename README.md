# electronics-verilator

The main contribution of this project is the `verilator.mk` makefile. Include it in your own project in the same manner as seen in the examples.

# Examples

The examples show how Verilator can be used to simualate a Verilog circuit. Feel free to add more examples and tests for them.

All examples can be run using the `makefile` at the top level of this repository.

## And

This is the most basic example. It shows the basic setup.

## Hierarchy

This example shows how a hierachy of modules can be setup and tested at unit and integration level. The top-level module instansiates a Delay module, which has its own folder and its own simulation for testing. The Delay module is then included by the top-level module and tested at that level as well. Also note that the top-level module includes another module Sub from another folder - kind of like a library.

# Step-by-step use

The user only needs to create three files, all of which can be copied and adapted from the examples.
* makefile
* PROJECT.sv
* sim.cpp

The variables in the makefile must be adapted to your project. The sim.cpp file needs the most work; the loop needs to be adapted so stimuli is applied and the expected results are checked.

# Waveform generation

Follow the example in And to see how waveforms are generated. This example generates a `trace.vcd` file, which can be opened using `gtkwave` like so: `gtkwave trace.vcd`.

There is nothing very fancy in the code - just copy it and use it in your own simulations to get waveform output. The `dump()` function should be called once every clock cycle / every time the circuit is evaluated.

# Continous Integration/Development

CI/CD is Continous Integration / Continous Development. GitHub Actions is a tool to implement this. The idea is to automate testing every time changes are made on GitHub - e.g., whenever a new pull request is set up, CICD will kick in to test the code before it can be merged.

See the file in `.github/workflows/makefile.yml`. It basically runs the long `docker` command, which essentially ends up calling make on this repository.
