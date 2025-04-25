# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
LDFLAGS = -pthread

# server program
TARGET1 = server
SRC1 = server_main.cpp server.cpp
OBJ1 = $(SRC1:.cpp=.o)

# client program
TARGET2 = client
SRC2 = client_main.cpp client.cpp
OBJ2 = $(SRC2:.cpp=.o)

# Build both programs
all: $(TARGET1) $(TARGET2)

# Rule for server program
$(TARGET1): $(OBJ1)
	$(CXX) $(OBJ1) $(LDFLAGS) -o $(TARGET1)

# Rule for client program
$(TARGET2): $(OBJ2)
	$(CXX) $(OBJ2) $(LDFLAGS) -o $(TARGET2)

# Compile .cpp files into .o files for server program
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp files into .o files for client program
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean server program
clean-program1:
	rm -f $(OBJ1) $(TARGET1)

# clean client program
clean-program2:
	rm -f $(OBJ2) $(TARGET2)

# Clean target: Remove all object files and executables
clean:
	rm -f $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)
