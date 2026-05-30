# Define variables
COMP = g++
CFLAGS = -Wall -g -Iinclude -Ithird_party
MAIN = main
TEST = test_suite
SRC_DIR = src
TEST_DIR = test
THIRD_PARTY_DIR = third_party
MAIN_SRCS = $(SRC_DIR)/main.cpp \
			$(SRC_DIR)/Door.cpp \
			$(SRC_DIR)/Renderer.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_SRCS = $(TEST_DIR)/test.cpp \
			$(SRC_DIR)/Door.cpp \
			$(SRC_DIR)/Renderer.cpp \
			$(THIRD_PARTY_DIR)/catch2/catch_amalgamated.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)


all: $(MAIN) $(TEST)

$(MAIN): $(MAIN_OBJS)
	$(COMP) $(CFLAGS) $^ -o $@ -lncurses

$(TEST): $(TEST_OBJS)
	$(COMP) $(CFLAGS) $^ -o $@ -lncurses

%.o: %.cpp
	$(COMP) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN) $(MAIN_OBJS) $(TEST) $(TEST_OBJS)