CXX=g++
CXXFLAGS=-I. -w -Wall -std=c++11

# Adjusting header file paths correctly
DEPS = options/option_class.hpp utils/utility_functions.hpp
OBJ = build/main.o build/exotic_options.o build/vanilla_options.o

# Linking all object files into the executable in the build directory
main: $(OBJ)
	$(CXX) -o build/main $^

# Pattern rule for compiling source files
build/%.o: %.cpp $(DEPS) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to clean up the build directory
.PHONY: clean
clean:
	rm -rf build

# Ensure correct .cpp source paths, possibly adding specific rules for different directories
build/main.o: main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/exotic_options.o: options/exotic_options.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/vanilla_options.o: options/vanilla_options.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
