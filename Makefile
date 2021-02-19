
# C++ Compiler and Flags
CXX=g++
CXX_FLAGS=-std=c++17 -O2 -g
CXX_WARN_FLAGS=-Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion
CXX_LINK_FLAGS=-lm

# Makefile rules
.PHONY:all 
all: pre-build bin/A bin/B bin/C 
	@$(MAKE) -s post-build

.PHONY:A
A: pre-build bin/A
	@$(MAKE) -s post-build

.PHONY:B
B: pre-build bin/B 
	@$(MAKE) -s post-build

.PHONY:C
C: pre-build bin/C
	@$(MAKE) -s post-build

bin/%: src/%/main.cpp
	@echo "## Compiling $<"
	@$(CXX) $(CXX_FLAGS) $(CXX_WARN_FLAGS) -o $@.out $< $(CXX_LINK_FLAGS)

.PHONY: pre-build
pre-build: 
	@echo "## Compiling source files..."
	@mkdir -p bin

.PHONY:post-build
post-build:
	@echo "## Finished!"
	
clean:
	@rm -rf bin
