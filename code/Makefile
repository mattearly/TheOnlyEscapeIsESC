# compiles any number of openGL C++ (.cpp) files
#         to be linked to a single executable

# Assumes the following file exists in the proper place.
# Pulls in OPTIONS, LDLIBS, and InitShader macros
include $(HOME)/projects/schoolOGL/Makefile.defs #mje linux location
#include /home/cs425/code-egs/Makefile.defs

TARGET = prog3_me298414_hauntedhouse_adventure_game
RUN = ./$(TARGET)
OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))

.PHONY: default
default:
	+$(MAKE) $(TARGET)

.PHONY: all
all:
	+$(MAKE) $(TARGET)

#links all the objects together to make the executable
$(TARGET): $(OBJECTS)
	# $(CXX) $(OBJECTS) $(InitShader) $(LDLIBS) -o $(TARGET)
	$(CXX) $(OBJECTS) $(LDLIBS) -o $(TARGET)  # removed init shader, using my own shader.h file

#compiles individual objects
%.o: %.cpp
	$(CXX) -c $(OPTIONS) $(LDLIBS) $< -o $@

.PHONY: clean
clean:
	-rm -f $(OBJECTS) $(TARGET) *core* *~

.PHONY: rebuild
rebuild:
	+$(MAKE) clean
	+$(MAKE) default

.PHONY: run
run:
	+$(MAKE) default
	$(RUN)

.PHONY: help
help:
	@echo ""
	@echo "----ABOUT MAKE COMMANDS----"
	@echo "make         - builds/updates everything, is ready to run with ./$(TARGET) after completion"
	@echo "make clean   - removes objects and executable"
	@echo "make rebuild - removes objects and executable and then builds and updates everything"
	@echo "make run     - builds/updates everything, runs as soon as possible"
	@echo ""

.PHONY: run-cygwin
run-cygwin:
	+$(MAKE) all
	startxwin $(RUN)


# fail cases:
# 1. couldn't find <glm/glm.hpp> 
#    linux mint 18 solution: apt install libglm-dev 
