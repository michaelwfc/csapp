/* $begin fdprob1 */
#include "csapp.h"

int main()
{
    int fd1, fd2;

    fd1 = Open("foo.txt", O_RDONLY, 0); //fd1 =3
    printf("fd1 = %d\n", fd1);
    Close(fd1);
    fd2 = Open("baz.txt", O_RDONLY, 0); //fd2 =3
    printf("fd2 = %d\n", fd2);
    exit(0);
}
/* $end fdprob1 */
