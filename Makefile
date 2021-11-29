CC=g++
SRC_DIR=src
BUID_DIR=build
TARGET=main

# All cpp files in SRC_DIR 
SOURCES=$(shell find $(SRC_DIR) -type f -name *.cpp)
# All the cpp files in SRC_DIR, but with .cpp replaced by .o
# Source: https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
OBJECTS=$(patsubst $(SRC_DIR)/%,$(BUID_DIR)/%,$(SOURCES:.cpp=.o))
# Enable all warning, use C++11, Optimization level 3
CFLAGS=-Wall -std=c++11 -O3
# Use the header files inside the "include" folder
INC=-I include

$(TARGET): $(OBJECTS)
	@echo "------------------------"
	@echo "Linking"
	@echo "------------------------"
	
	$(CC) $^ -o $(TARGET)

	@echo "Done."
	@echo "Executable = $(TARGET)"

$(BUID_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "------------------------"
	@echo "Building"
	@echo "------------------------"

	@mkdir -p $(BUID_DIR)
	
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning"
	$(RM) -rf $(BUID_DIR) $(TARGET_DIR)

test: $(TARGET)
	@echo "Testing"
	./$(TARGET) test/NLDM_lib_max2Inp test/cleaned_iscas89_99_circuits/c17.isc 10 11

.PHONY: clean
