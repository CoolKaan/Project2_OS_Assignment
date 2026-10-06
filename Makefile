CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Werror -Iinclude

# Directory for object files
BUILD := build

# Object files shared by every executable
COMMON_OBJS := $(BUILD)/main.o $(BUILD)/parser.o $(BUILD)/chunk.o

# The three programs to build
TARGETS := firstfit bestfit quickfit

.PHONY: all clean

# "make" and "make all" both build the three programs
all: $(TARGETS)

firstfit: $(COMMON_OBJS) $(BUILD)/firstfit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bestfit: $(COMMON_OBJS) $(BUILD)/bestfit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

quickfit: $(COMMON_OBJS) $(BUILD)/quickfit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile any src/*.cpp into build/*.o
# -MMD -MP also writes .d files so changing a header triggers a rebuild
$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD):
	mkdir -p $(BUILD)

# Remove object files, dependency files and executables
clean:
	rm -rf $(BUILD) $(TARGETS)

# Pull in the auto-generated header dependencies (ignored if none exist yet)
-include $(wildcard $(BUILD)/*.d)