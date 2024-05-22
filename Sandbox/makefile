# Targets and Dirs
ENXUS_DIR = ../Enxus
ENXUS_BIN_DIR = $(ENXUS_DIR)/bin
ENXUS_VENDOR_DIR = $(ENXUS_DIR)/vendor


# Precompiled headers
PCH_DIR = $(ENXUS_DIR)/src
PCH_SRC = $(PCH_DIR)/pch.h
PCH_OUT = $(PCH_DIR)/pch.h.gch
PCH_FLAGS = -include $(PCH_SRC)


# OpenGL & Include Flags
ENXUS_INCLUDE_FLAGS = -I$(ENXUS_DIR)/src
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Core
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Event
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/ImGui
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Renderer
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Math

VENDOR_FLAGS = -I$(ENXUS_VENDOR_DIR)
IMGUI_FLAGS = -I$(ENXUS_VENDOR_DIR)/imgui

OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Compiler
CXX = g++
DEBUG_FLAGS = -g
CXXFLAGS = -std=gnu++17 $(DEBUG_FLAGS) -Wall -Wextra

#------------------ SANDBOX ------------------

# Sandbox sources dir
SRC_DIR = src
# Sandbox bin dir
BIN_DIR = bin
# Tests sourcesdir
TESTS_DIR = $(SRC_DIR)/tests

TESTS_INCLUDE_FLAGS = -I$(TESTS_DIR)



# CPP Files 
# Sandbox sources
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(TESTS_DIR)/*.cpp)


# OBJ Files
OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
#NEWOBJS := $(subst $(SRC_DIR)/, $(BIN_DIR)/, $(SOURCES:.cpp=.o))

all: $(OBJS)

# Build all the cpp files into .o files
# %.o match %.cpp and match %.h (e.g: utils.o utils.cpp utils.h)
# headers files are added as prerequisites to watch changes in those files too

# Build Tests
$(BIN_DIR)/%.o: $(TESTS_DIR)/%.cpp $(TESTS_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(TESTS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build Sandbox
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(TESTS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)


build: 
	@echo "\n---------------------- BUILDING SANDBOX -----------------------\n"
	@make clean
	@make all
	@echo "\n---------------------- FINISHED  -----------------------\n"
	
clean:
	rm -fr $(BIN_DIR)/*

