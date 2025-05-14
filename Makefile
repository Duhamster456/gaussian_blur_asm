CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L/usr/local/lib/libpng16.a -lpng16

SRC_DIR = src
BUILD_DIR = build

TARGET = gauss.elf

MAIN_SRC = $(SRC_DIR)/main.c
ASM_SRC = $(SRC_DIR)/g_blur.s
C_SRC = $(SRC_DIR)/g_blur.c
MAIN_OBJ = $(BUILD_DIR)/main.o
VARIABLE_OBJ = $(BUILD_DIR)/g_blur.o

o0 : $(BUILD_DIR) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(C_SRC) -O0 -c -o $(VARIABLE_OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(VARIABLE_OBJ) -o $(TARGET) $(LDFLAGS)

o1 : $(BUILD_DIR) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(C_SRC) -O1 -c -o $(VARIABLE_OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(VARIABLE_OBJ) -o $(TARGET) $(LDFLAGS)

o2 : $(BUILD_DIR) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(C_SRC) -O2 -c -o $(VARIABLE_OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(VARIABLE_OBJ) -o $(TARGET) $(LDFLAGS)

o3 : $(BUILD_DIR) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(C_SRC) -O3 -c -o $(VARIABLE_OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(VARIABLE_OBJ) -o $(TARGET) $(LDFLAGS)

ofast : $(BUILD_DIR) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(C_SRC) -Ofast -c -o $(VARIABLE_OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(VARIABLE_OBJ) -o $(TARGET) $(LDFLAGS)

$(MAIN_OBJ) : $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS)

$(BUILD_DIR) : 
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)