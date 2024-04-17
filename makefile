# Targets and Dirs
BIN_DIR = bin
TARGET = $(BIN_DIR)/main
INCLUDE_DIR = include
VENDOR_DIR = vendor
IMGUI_DIR = $(VENDOR_DIR)/imgui
SRC_DIR = src
PCH_DIR = include/pch
# Precompiled headers
PCH_SRC = $(PCH_DIR)/pch.h
PCH_OUT = $(PCH_DIR)/pch.h.gch
PCH_FLAGS = -include $(PCH_SRC)

TEST_DIR = tests

# OpenGL & Include Flags
INCLUDE_FLAGS = -I$(INCLUDE_DIR)
VENDOR_FLAGS = -I$(VENDOR_DIR)
IMGUI_FLAGS = -I$(IMGUI_DIR)
OPENGL_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lassimp -lXrandr -lXi -ldl

# Compiler
CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra

# CPP Files 
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(IMGUI_DIR)/*.cpp)
SOURCES += $(wildcard $(VENDOR_DIR)/*.cpp)
SOURCES += $(wildcard $(TEST_DIR)/*.cpp)


# OBJ Files
OBJS = $(addprefix $(BIN_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
#NEWOBJS := $(subst $(SRC_DIR)/, $(BIN_DIR)/, $(SOURCES:.cpp=.o))

all: $(TARGET)

#rebuild: 

# Build all the cpp files into .o files
# %.o match %.cpp and match %.h (e.g: utils.o utils.cpp utils.h)
# headers files are added as prerequisites to watch changes in those files too

# Build all cpp files inside src
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(INCLUDE_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build stb_image cpp file inside vendor 
$(BIN_DIR)/%.o: $(VENDOR_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 


# Build imgui
$(BIN_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(IMGUI_FLAGS)

# Build tests
$(BIN_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) -c $< -o $@ $(INCLUDE_FLAGS) $(IMGUI_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)

# Build main
$(TARGET): $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $(PCH_FLAGS) $^ -o $@ $(INCLUDE_FLAGS) $(IMGUI_FLAGS) $(VENDOR_FLAGS) $(OPENGL_FLAGS)
	

run: all
	@echo "\n---------------------- RUNING -----------------------\n"
	@$(TARGET)
	@echo "\n------------------- END OF RUNING -------------------\n"	

pepe: pepe.cpp
	g++ -std=gnu++17 -Wall -Wextra pepe.cpp -o pepe && ./pepe

rebuild:
	@echo "\n---------------------- RE-BUILDING THE ENTIRE PROJECT -----------------------\n"
	@make clean
	@make all
	@echo "\n---------------------- FINISHED THE RE-BUILDING OF THE ENTIRE PROJECT -----------------------\n"

pch:
	$(CXX) $(CXXFLAGS) $(PCH_SRC) 


test:
	@cp tests/TestTemplate.h tests/TestNewFeature.h
	@cp tests/TestTemplate.cpp tests/TestNewFeature.cpp

clean:
	rm -fr $(BIN_DIR)/*

