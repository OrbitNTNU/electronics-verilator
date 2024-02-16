VERILATOR_ROOT := ../..
PROJECT_NAME := States
SOURCES := *.sv
SIMFILES := *.cpp
ENUMPARSE := ../../enumparse.py

include $(VERILATOR_ROOT)/verilator.mk

all:: enums

clean::
	rm -rf enums

enums:
	python3 $(ENUMPARSE) $(PROJECT_NAME)
