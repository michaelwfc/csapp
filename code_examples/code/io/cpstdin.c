/* 
strace -e trace=read ./cpstdin
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220q\0\0\0\0\0\0"..., 832) = 832
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\300A\2\0\0\0\0\0"..., 832) = 832
read(0, 123456
"1", 1)                         = 1
1read(0, "2", 1)                         = 1
2read(0, "3", 1)                         = 1
3read(0, "4", 1)                         = 1
4read(0, "5", 1)                         = 1
5read(0, "6", 1)                         = 1
6read(0, "\n", 1)                        = 1

read(0, 


strace -e trace=write ./cpstdin
123456 
write(1, "1", 11)                        = 1
write(1, "2", 12)                        = 1
write(1, "3", 13)                        = 1
write(1, "4", 14)                        = 1
write(1, "5", 15)                        = 1
write(1, "6", 16)                        = 1
write(1, "\n", 1
)                       = 1


*/
#include "csapp.h"

int main(void)
{
    char c;
    // int n=0;

    while (Read(STDIN_FILENO, &c, 1) != 0)
    {
        // n++;
        // printf("input count: %d\n", n);
        Write(STDOUT_FILENO, &c, 1);
    }
    exit(0);
}
/* $end cpstdin */
