# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDFLAGS = -pthread

# server program
TARGET1 = bin/server
SRCDIR1 = server
SRC1 = $(SRCDIR1)/server_main.cpp
OBJ1 = $(SRC1:.cpp=.o)

# client program
TARGET2 = bin/client
SRCDIR2 = client
SRC2 = $(SRCDIR2)/client_main.cpp
OBJ2 = $(SRC2:.cpp=.o)

# Build both programs
all: $(TARGET1) $(TARGET2)

# Rule for server program
$(TARGET1): $(OBJ1)
	mkdir -p bin
	$(CXX) $(OBJ1) $(LDFLAGS) -o $(TARGET1)

# Rule for client program
$(TARGET2): $(OBJ2)
	mkdir -p bin
	$(CXX) $(OBJ2) $(LDFLAGS) -o $(TARGET2)

# Compile client .cpp files into .o files (object files next to sources)
server/%.o: server/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile server .cpp files into .o files (object files next to sources)
client/%.o: client/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean server program
clean-program1:
	rm -f $(OBJ1) $(TARGET1)

# Clean client program
clean-program2:
	rm -f $(OBJ2) $(TARGET2)

# Clean all
clean:
	rm -f $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)
