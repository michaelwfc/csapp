#CC = /usr/local/cs/bin/gcc
# make
# 当你使用 make 命令时，Makefile 中定义的规则会被解析并执行。
# 根据你提供的 Makefile 内容，make 命令会默认执行第一个目标（通常是文件列表中的第一个目标）。
# 在你的 Makefile 中，第一个目标是 files，它依赖于多个文件和规则。

# gcc -Og  -S -m64 bufdemo.c -o bufdemo.64s
# gcc -Og  -m64 bufdemo.c -o bufdemo
# objdump -d bufdemo > bufdemo.64d
# rm -f bufdemo
# gcc -Og -fno-stack-protector  -S -m64 bufdemo.c -o bufdemo.64ns
# gcc -Og -fno-stack-protector  -m64 bufdemo.c -o bufdemo
# objdump -d bufdemo > bufdemo.64nd
# rm -f bufdemo
# gcc -Og  -m64 bufdemo.c -o bufdemo-sp
# gcc -Og -fno-stack-protector  -m64 bufdemo.c -o bufdemo-nsp


# CC: 指定使用的 C 编译器，默认为 gcc。
# CFLAGS: 编译选项，使用 -Og 进行基本优化并保持调试信息。
# CINC: 包含路径（当前为空）。
# F64: 使用 -m64 选项编译 64 位代码。
# FNOSP: 使用 -fno-stack-protector 禁用堆栈保护机制。


CC = gcc
CFLAGS= -Og
CINC =
F64 =-m64
FNOSP=-fno-stack-protector

# 后缀规则
# 定义了后缀规则，允许从 .c 文件生成 .64s, .64d, .64ns, 和 .64nd 文件
.SUFFIXES: .c .64s .64d .64ns .64nd

.c.64s:
	$(CC) $(CFLAGS) $(CINC) -S $(F64) $*.c -o $*.64s

.c.64d:
	$(CC) $(CFLAGS) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64d
	rm -f $*

.c.64ns:
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) -S $(F64) $*.c -o $*.64ns

.c.64nd:
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) $(F64) $*.c -o $*
	objdump -d $* > $*.64nd
	
# 目标规则
# files 是一个虚拟目标，依赖于多个文件。当运行 make files 时，会依次生成这些文件
files:	locate64 bufdemo.64s bufdemo.64d bufdemo.64ns bufdemo.64nd bufdemo-sp bufdemo-nsp

locate64: locate.c
	$(CC) $(CFLAGS) $(CINC) $(F64) locate.c -o locate64

bufdemo-sp: bufdemo.c
	$(CC) $(CFLAGS) $(CINC) $(F64) bufdemo.c -o bufdemo-sp

bufdemo-nsp: bufdemo.c
	$(CC) $(CFLAGS) $(FNOSP) $(CINC) $(F64) bufdemo.c -o bufdemo-nsp

# 清理规则
clean:
	rm -f *~ bufdemo-* *.64s *.64d *.64ns *.64nd



