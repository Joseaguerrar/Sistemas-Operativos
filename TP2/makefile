CXX = g++
CXXFLAGS = -Wall -std=c++17 -I.

# Carpetas
OBJDIR = obj
BINDIR = exec

# Archivos fuente y objetos
SRCS = main.cpp PageReplacement.cpp Algorithms.cpp
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

# Nombre del ejecutable
TARGET = $(BINDIR)/simulador

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/*

.PHONY: all clean
