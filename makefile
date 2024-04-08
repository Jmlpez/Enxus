# Targets and Dirs
BIN_DIR = bin
TARGET = $(BIN_DIR)/main
INCLUDE_DIR = include
IMGUI_DIR = imgui
SRC_DIR = src
TEST_DIR = $(SRC_DIR)/tests

# OpenGL & Include Flags
INCLUDE_FLAGS = -Iinclude
IMGUI_FLAGS = -Iimgui
OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl

# Compiler
CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra

# CPP Files 
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)
SOURCES += $(wildcard $(TEST_DIR)/*.cpp)


# OBJ Files
OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
#NEWOBJS := $(subst $(SRC_DIR)/, $(BIN_DIR)/, $(SOURCES:.cpp=.o))

all: $(TARGET)

# Build all the cpp files into .o files
# %.o match %.cpp and match %.h (e.g: utils.o utils.cpp utils.h)
# headers files are added as prerequisites to watch changes in those files too
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_FLAGS) $(IMGUI_FLAGS) $(OPENGL_FLAGS)

$(BIN_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(IMGUI_FLAGS)

$(BIN_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_FLAGS) $(IMGUI_FLAGS) $(OPENGL_FLAGS)

$(TARGET): $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(INCLUDE_FLAGS) $(IMGUI_FLAGS)  $(OPENGL_FLAGS)
	

run: all
	@echo "\n---------------------- RUNING -----------------------\n"
	@$(TARGET)
	@echo "\n------------------- END OF RUNING -------------------\n"	

clean:
	rm -fr $(BIN_DIR)/*