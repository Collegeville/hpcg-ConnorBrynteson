# Compiler
MPICXX = mpicxx
CXXFLAGS = -Wall -O2 -std=c++11 -fopenmp -g

# Include directories
INCLUDES = -I.

# Source files
SRCS = BuildingResult.cpp \
       CompareGeo.cpp \
       TestSuit.cpp \
       ../GenerateProblem_ref.cpp \
       ../GenerateProblem.cpp \
       ../ComputeSPMV_ref.cpp \
       ../ComputeSPMV.cpp \
       ../ExchangeHalo.cpp

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
