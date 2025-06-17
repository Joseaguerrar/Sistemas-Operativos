# Executable name and folders
TARGET = scheduler
BINDIR = exec
OBJDIR = obj
SRCDIR = src

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(SRCDIR)

# Find all source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Convert .cpp paths to .o in obj/
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Default rule
all: $(BINDIR)/$(TARGET)

# Create the executable from the .o
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp to .o inside obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create folders if they do not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Run the program
run: all
	./$(BINDIR)/$(TARGET)

# Clean objects and executable
clean:
	rm -rf $(OBJDIR) $(BINDIR)