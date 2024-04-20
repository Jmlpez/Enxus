# Targets and Dirs
BIN_DIR = bin
TARGET = $(BIN_DIR)/main
ENXUS_DIR = Enxus
ENXUS_VENDOR_DIR = Enxus/vendor
SANDBOX_DIR = Sandbox

# Enxus and Sandbox binaries
ENXUS_BIN_DIR = $(ENXUS_DIR)/bin
SANDBOX_BIN_DIR = $(SANDBOX_DIR)/bin


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

SANDBOX_FLAGS = -I$(SANDBOX_DIR)/src/
OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Compiler
CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra

# OBJ Files
#OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
OBJS = $(wildcard $(ENXUS_BIN_DIR)/*.o)
OBJS += $(wildcard $(SANDBOX_BIN_DIR)/*.o)

# Enxus Dependencies
ENXUS_DEPS = $(wildcard $(ENXUS_DIR)/src/Enxus/Core/*.cpp)
ENXUS_DEPS += $(wildcard $(ENXUS_DIR)/src/Enxus/ImGui/*.cpp)
ENXUS_DEPS += $(wildcard $(ENXUS_DIR)/src/Enxus/Renderer/*.cpp)

# Sandbox Dependencies
SANDBOX_DEPS = $(wildcard $(SANDBOX_DIR)/src/*.cpp)

# OBJ Files
ENXUS_OBJS = $(addsuffix .o, $(basename $(notdir $(ENXUS_DEPS))))
SANDBOX_OBJS = $(addsuffix .o, $(basename $(notdir $(SANDBOX_DEPS))))


all: $(TARGET)

# Build main
$(TARGET): $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) $^ -o $@ $(ENXUS_INCLUDE_FLAGS) $(VENDOR_FLAGS) $(SANDBOX_FLAGS) $(OPENGL_FLAGS)

enxus:
	$(MAKE) -C $(ENXUS_DIR)
sandbox:
	$(MAKE) -C $(SANDBOX_DIR)


run: all enxus sandbox
	@echo "\n---------------------- RUNING -----------------------\n"
	@$(TARGET)
	@echo "\n------------------- END OF RUNING -------------------\n"	

pepe: pepe.cpp
	g++ -std=gnu++17 -Wall -Wextra pepe.cpp -o pepe && ./pepe

# rebuild:
# 	@echo "\n---------------------- RE-BUILDING THE ENTIRE PROJECT -----------------------\n"
# 	@make clean
# 	@make all
# 	@echo "\n---------------------- FINISHED THE RE-BUILDING OF THE ENTIRE PROJECT -----------------------\n"

# pch:
# 	$(CXX) $(CXXFLAGS) $(PCH_SRC) 


# test:
# 	@cp tests/TestTemplate.h tests/TestNewFeature.h
# 	@cp tests/TestTemplate.cpp tests/TestNewFeature.cpp

clean:
	rm -fr $(BIN_DIR)/*
clean-full:
	$(MAKE) clean
	$(MAKE) -C $(ENXUS_DIR) clean
	$(MAKE) -C $(SANDBOX_DIR) clean

