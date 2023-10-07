# 使用的编译器
CC = gcc

# 编译器选项, -g 表示生成调试信息, -Wall 表示开启所有警告
# 假设 log.h 位于 log/include 目录下
CFLAGS = -g -Wall -I../include 

# 链接时需要的库选项
LDFLAGS = -lsqlite3 -lbcrypt -lncursesw

# 源代码文件列表
SOURCES = ../utils/src/network.c client.c ../log/log.c  # 更新源代码文件列表，加入 log.c 的完整路径

# 头文件列表
# 假设 log.h 位于 log/include 目录下
HEADERS = client.h # 更新头文件列表，加入 log.h 的完整路径

# 程序的可执行文件名
PROGRAM = bin/client

# 对象文件列表，这里的前缀是 bin/ ，使得所有的对象文件都被创建在 bin/ 下
OBJECTS = $(addprefix bin/, $(notdir $(SOURCES:.c=.o)))

# 指定源文件的搜索路径
# 加入 ../log/src 以确保 log.c 能被找到
VPATH = src:../utils/src:../log


# 默认目标
all: directories $(PROGRAM)

# 创建目标目录
directories:
	@mkdir -p bin

# 链接最终程序
$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 编译源代码文件，这里为 -o 选项指定了路径前缀 bin/
# 更新依赖项，确保包含 log.h
bin/%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

# 清除目标，删除所有生成的文件
clean:
	rm -f $(OBJECTS) $(PROGRAM)