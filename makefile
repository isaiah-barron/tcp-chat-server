# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall

# server program
TARGET1 = server
SRC1 = server.cpp
OBJ1 = $(SRC1:.cpp=.o)

# client program
TARGET2 = client
SRC2 = client.cpp
OBJ2 = $(SRC2:.cpp=.o)

# Build both programs
all: $(TARGET1) $(TARGET2)

# Rule for server program
$(TARGET1): $(OBJ1)
	$(CXX) $(OBJ1) -o $(TARGET1)

# Rule for client program
$(TARGET2): $(OBJ2)
	$(CXX) $(OBJ2) -o $(TARGET2)

# Compile .cpp files into .o files for server program
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp files into .o files for client program
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target: Remove object files and executables
clean:
	rm -f $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)
