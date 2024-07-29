CXX = g++

PROJECT_NAME = Sandbox

SOURCE_DIR = Source
OUTPUT_DIR = Binaries
LIBRARY_DIR = ./../JadeEngine/Libraries

CXXFLAGS = -std=c++17 -MD -Wall -Wextra -Werror -ggdb
LDFLAGS = 
DEFINES = -D_DEBUG
INCLUDES = -ISource -I./../JadeEngine/Source

LIBDIRS = -L./../JadeEngine/Binaries

LIBRARIES = -lJadeEngine 
LIBRARIES += -lWs2_32

# LIBDIRS += -LJadeEngine/Libraries/glad
# LIBRARIES += -lglad -lgdi32 -lopengl32

LIBDIRS += -L"$(VK_SDK_PATH)/Lib"
LIBRARIES += -lvulkan-1

# LIBDIRS += -L"$(DXSDK_DIR)/Lib/x64"
# LIBRARIES += -ld3d11 -ldxgi -ld3dcompiler -ldxguid -ld3d11sdklayers

INCLUDES += -I./../JadeEngine/Libraries/glm

SOURCES = $(wildcard $(SOURCE_DIR)/**.cpp $(SOURCE_DIR)/**/**.cpp $(SOURCE_DIR)/**/**/**.cpp $(SOURCE_DIR)/**/**/**/**.cpp)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(OUTPUT_DIR)/Intermediate/%.o)

.PHONY: all

all: Sandbox

Sandbox: $(OBJECTS)
	$(CXX) -o $(OUTPUT_DIR)/$(PROJECT_NAME).exe $^ $(LIBDIRS) $(LDFLAGS) $(LIBRARIES)

$(OUTPUT_DIR)/Intermediate/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES) $(DEFINES)
	
clean:
	cd Binaries && del /s /q *
	
-include $(OBJECTS:.o=.d)