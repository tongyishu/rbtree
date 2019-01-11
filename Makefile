CC = gcc
# 可执行文件
TARGET = test
# C文件
SRCS = test.c rbtree.c
# 目标文件
OBJS = $(SRCS:.c=.o)
# 库文件
DLIBS =
# 链接为可执行文件
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(DLIBS)  
clean:
	rm -rf $(TARGET) $(OBJS)
# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.c
	$(CC) -o $@ -c -g $<
