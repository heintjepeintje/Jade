CXX = g++

SOURCE_DIR = Source
OUTPUT_DIR = Binaries
LIBRARY_DIR = Libraries
PROJECT_NAME = JadeEngine

CXXFLAGS = -std=c++23 -MD -Wall -Wextra
DEFINES = -D_DEBUG
INCLUDES = -I$(SOURCE_DIR)

# INCLUDES += -I$(VK_SDK_PATH)/Include
# INCLUDES += -I"$(DXSDK_DIR)/Include"

SOURCES = $(wildcard $(SOURCE_DIR)/**.cpp $(SOURCE_DIR)/**/**.cpp $(SOURCE_DIR)/**/**/**.cpp $(SOURCE_DIR)/**/**/**/**.cpp)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(OUTPUT_DIR)/Intermediate/%.o)

.PHONY: all

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJECTS)
	ar rcs $(OUTPUT_DIR)/$(PROJECT_NAME).lib $^ -s

$(OUTPUT_DIR)/Intermediate/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -o $@ -ggdb -c $< $(CXXFLAGS) $(INCLUDES) $(DEFINES)
	
clean:
	cd Binaries && del /s /q *

-include $(OBJECTS:.o=.d)