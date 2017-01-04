SOURCE_DIR := src/
BUILD_DIR  := build/

SOURCES += $(wildcard $(SOURCE_DIR)*.c)
SOURCES += $(wildcard $(SOURCE_DIR)*.cpp)
OBJECTS += $(patsubst $(SOURCE_DIR)%c,    $(BUILD_DIR)%.o, $(filter %.c,   $(SOURCES)))
OBJECTS += $(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)%.o, $(filter %.cpp, $(SOURCES)))

TEST_TARGET := $(BUILD_DIR)bigint

test: $(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

$(TEST_TARGET): $(OBJECTS) $(BUILD_DIR)
	g++ -g -o $@ $(OBJECTS)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.c $(BUILD_DIR)
	gcc -g -c -o $@ $<

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp $(BUILD_DIR)
	g++ -g -c -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
