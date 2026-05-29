# Define variables
COMP = g++
CFLAGS = -Wall -g
TARGET = main
SRCS = main.cpp Door.cpp Renderer.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(COMP) $(CFLAGS) $(OBJS) -o $(TARGET) -lncurses

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)