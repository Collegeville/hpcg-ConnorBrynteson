# Compiler
MPICXX = mpicxx
CXXFLAGS = -Wall -O2 -std=c++11

# Include directories
INCLUDES = -I.

# Source files
SRCS = BuildingResult.cpp \
       CompareGeo.cpp \
       TestSuit.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
EXEC = test_suite

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(MPICXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(MPICXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up the build directory
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
