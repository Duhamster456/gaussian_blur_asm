CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L/usr/local/lib/libpng16.a -lpng16

SRC_DIR = src
BUILD_DIR = build

TARGET = gauss.elf

SRCS = $(SRC_DIR)/main.c
OBJS = $(BUILD_DIR)/main.o

all : $(BUILD_DIR) $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) : 
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)