
VERILATOR_EXE := obj_dir/V$(PROJECT_NAME)

PARSER ?= $(VERILATOR_ROOT)/default_parser.sh

VERILATOR_ARGS += --cc --build --exe --trace

all: run

lint: $(SOURCES)
	verilator --lint-only -Wall $^ --top-module $(PROJECT_NAME)

$(VERILATOR_EXE): lint | $(SOURCES)
	verilator $(VERILATOR_ARGS) $(SIMFILES) $(SOURCES) > /dev/null

run: $(VERILATOR_EXE)
	# https://stackoverflow.com/questions/17757039/equivalent-of-pipefail-in-dash-shell
	@mkfifo named_pipe
	@tee output.txt < named_pipe &
	@./$(VERILATOR_EXE) > named_pipe; ./$(PARSER) $(PROJECT_NAME) $$? output.txt

clean:
	rm -rf obj_dir

