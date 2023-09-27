# 定义编译器
CC = gcc

# 定义编译标志
CFLAGS = -Wall -Isrc

# 定义链接标志，-lncurses用于链接ncurses库
LDFLAGS = -lncursesw 

# 定义源文件和目标文件
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

# 目标执行文件名
TARGET = GomokuGame

# 默认目标
all: $(TARGET)

# 生成目标执行文件
$(TARGET): $(OBJ_FILES)
	$(CC)  $^ -o $@ $(CFLAGS) $(LDFLAGS)

# 生成目标对象文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清除生成的文件
clean:
	rm -f *.o $(TARGET)

