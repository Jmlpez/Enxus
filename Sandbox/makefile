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

VENDOR_FLAGS = -I$(ENXUS_VENDOR_DIR)
IMGUI_FLAGS = -I$(ENXUS_VENDOR_DIR)/imgui

OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Compiler
CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra


# Sandbox sources dir
SRC_DIR = src
# Sandbox bin dir
BIN_DIR = bin

# CPP Files 
# Sandbox sources
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)


# OBJ Files
OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
#NEWOBJS := $(subst $(SRC_DIR)/, $(BIN_DIR)/, $(SOURCES:.cpp=.o))

all: $(OBJS)

# Build all the cpp files into .o files
# %.o match %.cpp and match %.h (e.g: utils.o utils.cpp utils.h)
# headers files are added as prerequisites to watch changes in those files too

# Build Sandbox
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)


build: 
	@echo "\n---------------------- BUILDING SANDBOX -----------------------\n"
	@make clean
	@make all
	@echo "\n---------------------- FINISHED  -----------------------\n"
	
clean:
	rm -fr $(BIN_DIR)/*
