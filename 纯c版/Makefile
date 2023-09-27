# 定义编译器
CC = gcc

# 定义编译标志，添加-I标志以指定头文件目录
CFLAGS = -Wall -Isrc

# 定义源文件和目标文件
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*/*.c $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

# 目标执行文件名
TARGET = GomokuGame

# 默认目标
all: $(TARGET)

# 生成目标执行文件
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# 生成目标对象文件
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清除生成的文件
clean:
	rm -f $(SRC_DIR)/*/*.o $(SRC_DIR)/*.o $(TARGET)
