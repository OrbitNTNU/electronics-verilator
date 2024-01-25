# Override this to use the makefile for other projects too
SEARCH ?= examples

MAKES := $(shell find $(SEARCH) -mindepth 1 -name makefile)
TEST_FOLDERS := $(subst makefile,, $(MAKES))

all: run

run:
	set -e && \
	for test in $(TEST_FOLDERS) ; do \
		make -C $$test all ; \
	done

clean:
	for test in $(TEST_FOLDERS) ; do \
		make -C $$test clean ; \
	done
