#
# Students' Makefile for the Malloc Lab
#
TEAM = bovik
VERSION = 1
HANDINDIR = /afs/cs.cmu.edu/academic/class/15213-f01/malloclab/handin

# 用户正在编译一个 C 文件（mdriver.c），并指定了 -m32 参数，表明目标是生成 32 位代码。
# 如果系统是 64 位 Linux，但缺少 32 位开发库（如 glibc 的 32 位支持），会导致编译失败。

# 某些发行版（如 Ubuntu）需要显式安装 32 位开发工具链才能支持 -m32 编译。
# dpkg --print-architecture
# dpkg --print-foreign-architectures
# 如果输出中没有 i386，说明系统未启用 32 位架构支持

# sudo apt update
# sudo apt install gcc-multilib g++-multilib libc6-dev-i386
# 这些包提供了 GCC 的 32 位支持以及 Glibc 的 32 位头文件和库。


CC = gcc

# CFLAGS = -Wall -O2 
#-m32
# for debug
CFLAGS = -g

OBJS = mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o

mdriver: $(OBJS)
	$(CC) $(CFLAGS) -o mdriver $(OBJS)

mdriver.o: mdriver.c fsecs.h fcyc.h clock.h memlib.h config.h mm.h
memlib.o: memlib.c memlib.h
mm.o: mm.c mm.h memlib.h
fsecs.o: fsecs.c fsecs.h config.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h

handin:
	cp mm.c $(HANDINDIR)/$(TEAM)-$(VERSION)-mm.c

clean:
	rm -f *~ *.o mdriver


