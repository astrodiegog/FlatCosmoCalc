SHELL = /usr/bin/env bash

#-- Set default include makefile
TYPE    ?= default
HOST 	?= dag

#-- Set the stuffix 
SUFFIX ?= .$(TYPE).$(HOST)

#-- Load any flags from TYPE & HOST
include builds/make.type.$(TYPE)
include builds/make.host.$(HOST)

#-- Directories to: find source files, build objects, and place binary
SRC_DIRS := src src/integrator src/cosmology
BUILD_DIR := build
BIN_DIR := bin

#-- Place all c files into a var
CPPFILES := $(foreach DIR,$(SRC_DIRS),$(wildcard $(DIR)/*.cpp))

#-- Prepend BUILD_DIR and appends .o to all source C files
CLEAN_OBJS   := $(CPPFILES:%=$(BUILD_DIR)/%.o)

#-- In the case any CPPFILES changes
OBJS   := $(CPPFILES:%=$(BUILD_DIR)/%.o)

#-- Set default compiler and flags
CXX               ?= CXX
CXXFLAGS_OPTIMIZE ?= -g -Ofast -std=c++17
CXXFLAGS_DEBUG    ?= -g -O0 -std=c++17 -Wall

BUILD             ?= DEBUG

#-- Grab the appropriate CPPFLAGS
CXXFLAGS          += $(CXXFLAGS_$(BUILD))

#-- Add flags and libraries as needed
CXXFLAGS += $(DFLAGS) -Isrc

#-- Define the executable
EXEC := bin/flatcosmocalc$(SUFFIX)

#-- Label flags
MACRO_FLAGS := -DMACRO_FLAGS='"$(DFLAGS)"'
DFLAGS      += $(MACRO_FLAGS)

#-- Define the link editor & flags
LD := $(CXX)
LDFLAGS := $(CXXFLAGS)

#-- Execute the target executable once prereq-build is set
$(EXEC): prereq-build $(OBJS)
	echo "We are linking tings!"
	echo $(LDFLAGS)
	mkdir -p bin/ && $(LD) $(LDFLAGS) $(OBJS) -o $(EXEC)

#-- Build step for the C source code
$(BUILD_DIR)/%.cpp.o: %.cpp
	echo "We are building object files!"
	mkdir -p $(dir $@)
	# echo $(CXX)
	$(CXX) $(CXXFLAGS) -c $< -o $@

foo += more

.PHONY: clean

all: 
	echo $(foo)
	echo $(DFLAGS)
	echo $(CFILES)
	echo $(OBJS)
	echo $(OBJS1)


prereq-build:
	echo "We are in prereq-build!"
	builds/prereq.sh build $(HOST)


clean:
	rm -f $(CLEAN_OBJS)
	rm -r $(BUILD_DIR)
