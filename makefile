# Targets and Dirs
BIN_DIR = bin
SANDBOX_TARGET = $(BIN_DIR)/main
TERRAIN_GEN_TARGET = $(BIN_DIR)/Terrain
ENXUS_DIR = Enxus
ENXUS_VENDOR_DIR = Enxus/vendor
SANDBOX_DIR = Sandbox
TERRAIN_GEN_DIR = TerrainGen

# Enxus and Sandbox binaries
ENXUS_BIN_DIR = $(ENXUS_DIR)/bin
SANDBOX_BIN_DIR = $(SANDBOX_DIR)/bin
TERRAIN_GEN_BIN_DIR = $(TERRAIN_GEN_DIR)/bin


# OpenGL & Include Flags 
ENXUS_INCLUDE_FLAGS = -I$(ENXUS_DIR)/src
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Core
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Event
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/ImGui
ENXUS_INCLUDE_FLAGS += -I$(ENXUS_DIR)/src/Enxus/Renderer

VENDOR_FLAGS = -I$(ENXUS_VENDOR_DIR)
VENDOR_FLAGS += -I$(ENXUS_VENDOR_DIR)/imgui
VENDOR_FLAGS += -I$(ENXUS_VENDOR_DIR)/stb_image
VENDOR_FLAGS += -I$(ENXUS_VENDOR_DIR)/debugbreak

SANDBOX_FLAGS = -I$(SANDBOX_DIR)/src
TERRAIN_GEN_FLAGS = -I$(TERRAIN_GEN_DIR)/src
OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Precompiled headers
PCH_DIR = $(ENXUS_DIR)/src
PCH_SRC = $(PCH_DIR)/pch.h
PCH_FLAGS = -include $(PCH_SRC)

# Compiler
CXX = g++
DEBUG_FLAGS = -g
CXXFLAGS = -std=gnu++17 $(DEBUG_FLAGS) -Wall -Wextra

# OBJ Files
#OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
ENXUS_OBJS = $(wildcard $(ENXUS_BIN_DIR)/*.o)
SANDBOX_OBJS += $(wildcard $(SANDBOX_BIN_DIR)/*.o)
TERRAIN_GEN_OBJS += $(wildcard $(TERRAIN_GEN_BIN_DIR)/*.o)

sandbox-app: 
	$(MAKE) -C $(SANDBOX_DIR)
	$(MAKE) $(SANDBOX_TARGET)

terrain-app: 
	$(MAKE) -C $(TERRAIN_GEN_DIR)
	$(MAKE) $(TERRAIN_GEN_TARGET)

# Build sandbox target
$(SANDBOX_TARGET): $(ENXUS_OBJS) $(SANDBOX_OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) $^ -o $@  $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(TERRAIN_GEN_FLAGS) $(SANDBOX_FLAGS) $(OPENGL_FLAGS)
	
# Build terrain target
$(TERRAIN_GEN_TARGET): $(ENXUS_OBJS) $(TERRAIN_GEN_OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) $^ -o $@  $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(TERRAIN_GEN_FLAGS) $(SANDBOX_FLAGS) $(OPENGL_FLAGS)
	
enxus:
	$(MAKE) -C $(ENXUS_DIR) build
sandbox:
	$(MAKE) -C $(SANDBOX_DIR) build
terrain:
	$(MAKE) -C $(TERRAIN_GEN_DIR) build

rebuild:
	@echo "\n---------------------- RE-BUILDING THE ENTIRE PROJECT -----------------------\n"
	$(MAKE) enxus
	$(MAKE) sandbox
	$(MAKE) terrain
	$(MAKE) clean
	$(MAKE) $(SANDBOX_TARGET)
	$(MAKE) $(TERRAIN_GEN_TARGET)
	@echo "\n---------------------- FINISHED THE RE-BUILDING OF THE ENTIRE PROJECT -----------------------\n"

run-sandbox:
	$(MAKE) -C $(ENXUS_DIR)
	$(MAKE) sandbox-app
	@echo "\n---------------------- RUNING -----------------------\n"
	@$(SANDBOX_TARGET)
	@echo "\n------------------- END OF RUNING -------------------\n"	

#$(MAKE) terrain-app
run-terrain:
	$(MAKE) -C $(ENXUS_DIR)	
	$(MAKE) terrain-app
	@echo "\n---------------------- RUNING -----------------------\n"
	@$(TERRAIN_GEN_TARGET)
	@echo "\n------------------- END OF RUNING -------------------\n"	

# Testing purposes
pepe: pepe.cpp
	g++ -std=gnu++17 -Wall -Wextra pepe.cpp -o pepe && ./pepe


clean:
	rm -fr $(BIN_DIR)/*
clean-enxus:
	$(MAKE) -C $(ENXUS_DIR) clean
clean-sandbox:
	$(MAKE) -C $(SANDBOX_DIR) clean
clean-terrain:
	$(MAKE) -C $(TERRAIN_GEN_DIR) clean

clean-full: 
	$(MAKE) clean-enxus
	$(MAKE) clean-sandbox
	$(MAKE) clean-terrain
	$(MAKE) clean
	

