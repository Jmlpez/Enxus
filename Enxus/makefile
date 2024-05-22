# Targets and Dirs

BIN_DIR = bin

SRC_DIR = src
ENXUS_CORE_DIR = $(SRC_DIR)/Enxus/Core
ENXUS_EVENT_DIR = $(SRC_DIR)/Enxus/Event
ENXUS_IMGUI_DIR = $(SRC_DIR)/Enxus/ImGui
ENXUS_RENDERER_DIR = $(SRC_DIR)/Enxus/Renderer
ENXUS_MATH_DIR = $(SRC_DIR)/Enxus/Math

# Vendor Targets
VENDOR_DIR = vendor
IMGUI_DIR = $(VENDOR_DIR)/imgui
STB_IMAGE_DIR = $(VENDOR_DIR)/stb_image


# Precompiled headers
PCH_DIR = $(SRC_DIR)
PCH_SRC = $(PCH_DIR)/pch.h
PCH_OUT = $(PCH_DIR)/pch.h.gch
PCH_FLAGS = -include $(PCH_SRC)

# OpenGL & Include Flags
ENXUS_INCLUDE_FLAGS = -I$(SRC_DIR)
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_CORE_DIR)
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_EVENT_DIR)
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_IMGUI_DIR)
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_RENDERER_DIR)
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_MATH_DIR)

VENDOR_FLAGS = -I$(VENDOR_DIR)
IMGUI_FLAGS = -I$(IMGUI_DIR)
OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Compiler
CXX = g++
DEBUG_FLAGS = -O2
CXXFLAGS = -std=gnu++17 $(DEBUG_FLAGS) -Wall -Wextra

# CPP Files 
# Enxus sources
SOURCES = $(wildcard $(ENXUS_CORE_DIR)/*.cpp)
SOURCES += $(wildcard $(ENXUS_IMGUI_DIR)/*.cpp)
SOURCES += $(wildcard $(ENXUS_RENDERER_DIR)/*.cpp)
SOURCES += $(wildcard $(ENXUS_MATH_DIR)/*.cpp)

# Vendor sources
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)
SOURCES += $(wildcard $(STB_IMAGE_DIR)/*.cpp)



# OBJ Files
OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
#NEWOBJS := $(subst $(SRC_DIR)/, $(BIN_DIR)/, $(SOURCES:.cpp=.o))

all: $(OBJS)

# Build all the cpp files into .o files
# %.o match %.cpp and match %.h (e.g: utils.o utils.cpp utils.h)
# headers files are added as prerequisites to watch changes in those files too

# Build Enxus Core
$(BIN_DIR)/%.o: $(ENXUS_CORE_DIR)/%.cpp $(ENXUS_CORE_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)
	

# Build Enxus ImGui Layer
$(BIN_DIR)/%.o: $(ENXUS_IMGUI_DIR)/%.cpp $(ENXUS_IMGUI_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build Enxus Renderer
$(BIN_DIR)/%.o: $(ENXUS_RENDERER_DIR)/%.cpp $(ENXUS_RENDERER_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build Enxus Math Utils
$(BIN_DIR)/%.o: $(ENXUS_MATH_DIR)/%.cpp $(ENXUS_MATH_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build Imgui
$(BIN_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(IMGUI_FLAGS)

# Build stb_image 
$(BIN_DIR)/%.o: $(STB_IMAGE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

build:  
	@echo "\n---------------------- BUILDING ENXUS ENGINE -----------------------\n"
	@make clean
	@make all
	@echo "\n---------------------- FINISHED  -----------------------\n"
	
clean:
	rm -fr $(BIN_DIR)/*

