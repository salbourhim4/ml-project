CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

TARGET = ml-project

SRCS = src/matrix.c src/main.c src/mnist.c src/nn.c
OBJS = $(SRCS:src/%.c=build/%.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)