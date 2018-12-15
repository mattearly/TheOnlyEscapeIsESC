# compiles any number of openGL C++ (.cpp) files
#         to be linked to a single executable


TARGET = TheOnlyEscapeIsEscape
RUN = ./$(TARGET)
SRC_DIR = code
BUILD_DIR = bin
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
CFLAGS = -pedantic -std=c++11
LDLIBS = -lGL -lGLU -lglut -lGLEW -lSDL2main -lSDL2 -lSDL2_mixer
OPTIONS = $(CFLAGS) $(INCS) $(LDLIBS)

.PHONY: default
default:
	mkdir -p $(BUILD_DIR)
	+$(MAKE) $(TARGET)

#links all the objects together to make the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDLIBS) -o $(TARGET)  

#compiles individual objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
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
