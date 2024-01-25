
VERILATOR_EXE := obj_dir/V$(PROJECT_NAME)

PARSER ?= $(VERILATOR_ROOT)/default_parser.sh

VERILATOR_ARGS += --cc --build --exe --trace

all: run

lint: $(SOURCES)
	verilator --lint-only -Wall $^ --top-module $(PROJECT_NAME)

$(VERILATOR_EXE): lint | $(SOURCES)
	verilator $(VERILATOR_ARGS) $(SIMFILES) $(SOURCES) > /dev/null

run: $(VERILATOR_EXE)
	./$(VERILATOR_EXE) | tee output.txt; ./$(PARSER) $(PROJECT_NAME) $$? output.txt

clean:
	rm -rf obj_dir

