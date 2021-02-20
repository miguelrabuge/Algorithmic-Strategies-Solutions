
# C++ Compiler and Flags
CXX=g++
CXX_FLAGS=-std=c++17 -O2 -g
CXX_WARN_FLAGS=-Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion
CXX_LINK_FLAGS=-lm

# Makefile rules
.PHONY:all 
all: pre-build A B C

.PHONY:A
A: pre-build bin/A.out

.PHONY:B
B: pre-build bin/B.out

.PHONY:C
C: pre-build bin/C.out
	
bin/%.out: src/%/main.cpp
	@echo -n "## Compiling $<  "
	@$(CXX) $(CXX_FLAGS) $(CXX_WARN_FLAGS) -o $@ $< $(CXX_LINK_FLAGS)
	@echo "DONE!"

.PHONY: pre-build
pre-build: 
	@mkdir -p bin

clean:
	@rm -rf bin
