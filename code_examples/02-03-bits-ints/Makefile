CFLAGS = -Og -I../../src/utils
CC = gcc

# all: show-bytes32 show-bytes64

# show-bytes32: show-bytes.c
# 	$(CC) $(CFLAGS) -m32 show-bytes.c -o show-bytes32

# show-bytes64: show-bytes.c
# 	$(CC) $(CFLAGS) -m64 show-bytes.c -o show-bytes64

# Source files
SRCS  = show-bytes.c ../../src/utils/arrayUtils.c

# Object files: List of object files derived from the source files
OBJS = $(SRCS:.c=.o)

all: show-bytes64

# Linking Rule: Defines how to link the object files into the final executable
show-bytes64: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o show-bytes64 

# Compilation Rule: Defines how to compile each source file into an object file. 
# $< represents the input file, and $@ represents the output file.
# Rule to compile each source file into an object file
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# clean:
# 	rm -f *~ *.o show-bytes32 show-bytes64
