MAKEFILESS := $(shell find examples -mindepth 1 -name makefile)
TEST_FOLDERS := $(subst makefile,, $(MAKEFILESS))

all: run

run:
	for test in $(TEST_FOLDERS) ; do \
		make -C $$test all ; \
	done

clean:
	for test in $(TEST_FOLDERS) ; do \
		make -C $$test clean ; \
	done
