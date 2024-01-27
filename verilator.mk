MODULE_FOLDER ?= examples
PROJECT_ROOT ?= ${VERILATOR_ROOT}

# Find the path between the top level folder and the source code folder
PATH_FROM_ROOT_TO_SRC = $(shell p=$(shell pwd); g=$${p\#\#*/${MODULE_FOLDER}}; echo $$g)

VERILATOR := $(VERILATOR_ROOT)/verilator-docker.sh
VERILATOR_ARGS := 4.038 ${MODULE_FOLDER}${PATH_FROM_ROOT_TO_SRC} ${PROJECT_ROOT}
VERILATOR_ARGS_LINT     := --lint-only -Wall
VERILATOR_ARGS_EMULATOR := --cc --build --exe --trace

VERILATOR_EMULATOR := obj_dir/V$(PROJECT_NAME)
PARSER ?= $(VERILATOR_ROOT)/default_parser.sh

all: run

lint: $(SOURCES) $(GENERATED)
	./$(VERILATOR) $(VERILATOR_ARGS) $(VERILATOR_ARGS_LINT) $(SOURCES) --top-module $(PROJECT_NAME)

$(VERILATOR_EMULATOR): lint | $(SOURCES) $(GENERATED)
	./$(VERILATOR) $(VERILATOR_ARGS) $(VERILATOR_ARGS_EMULATOR) $(SIMFILES) $(SOURCES) > /dev/null

# https://stackoverflow.com/questions/17757039/equivalent-of-pipefail-in-dash-shell
run: $(VERILATOR_EMULATOR)
	@mkfifo named_pipe
	@tee output.txt < named_pipe &
	@./$(VERILATOR_EMULATOR) > named_pipe; ./$(PARSER) $(PROJECT_NAME) $$? output.txt
	@rm named_pipe

clean:
	@rm -rf obj_dir
	@rm -f named_pipe
