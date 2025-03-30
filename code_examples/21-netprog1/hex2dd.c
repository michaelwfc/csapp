#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <errno.h>
#include <limits.h>

// #include "csapp.h"
// uint32_t htonl(uint32_t hostlong);
// uint16_t htons(uint16_t hostshort);
// Returns: value in network byte order
// uint32_t ntohl(uint32_t netlong);
// uint16_t ntohs(unit16_t netshort);
// Returns: value in host byte order

// #include <arpa/inet.h>
// int inet_pton(AF_INET, const char *src, void *dst);
// Returns: 1 if OK, 0 if src is invalid dotted decimal, −1 on error
// const char *inet_ntop(AF_INET, const void *src, char *dst, socklen_t size);
// Returns: pointer to a dotted-decimal string if OK, NULL on error

/* IP address structure */
// struct in_addr
// {
//     uint32_t s_addr; /* Address in network byte order (big-endian) */
// };



/*
Practice Problem 11.1 (solution page 1002)
Complete the following table:
Dotted-decimal address       Hex address
107.212.122.205     107=0x6B, 212=0xD4, 122=0x7A, 205=0xCD  0x6BD47ACD
64.12.149.13
107.212.96.29
                    0x00000080
                    0xFFFFFF00
                    0x0A010140


Practice Problem 11.2 (solution page 1003)
Write a program hex2dd.c that converts its 16-bit hex argument to a 16-bit network byte order and prints the result.
Address in network byte order (big-endian)

For example
linux> ./hex2dd 0x400
1024

idea:
0x400 is 0x400 in hexadecimal, which is 1024 in decimal.
0x400 which is  0100 0000 0000 0000 in binary.
*/



int main(int argc, char *argv[])
{
    // 检查命令行参数数量是否正确
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <hex>\n", argv[0]);
        return 1; // 使用 return 替代 exit，便于未来扩展
    }
    uint16_t network_order_byte;
    long result = strtol(argv[1], NULL, 16); // 使用 long 类型避免溢出

    // 将结果强制转换为 unsigned short
    uint16_t hostshort = (uint16_t)result;

    network_order_byte = htons(hostshort);

    // 打印结果（如果需要）
    printf("Parsed network_order_byte value: %hu\n", network_order_byte);

    return 0;
}
