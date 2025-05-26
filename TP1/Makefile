CXX=g++
CXXFLAGS=-Wall -std=c++17
TARGET=exec/main
SRC=main.cpp utils.cpp scheduler.cpp

all: create_dir $(TARGET)

create_dir:
	@if [ ! -d exec ]; then mkdir exec; fi

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
