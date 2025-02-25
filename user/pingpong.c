#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int fd1[2], fd2[2]; //fd = file discriptor
    
    pipe(fd1); 
    pipe(fd2);

    char byte_send = '.';
    char byte_received;

    if (fork() == 0)
    {
        close(fd1[1]);
        read(fd1[0], &byte_received, 1);
        if (byte_received == '.')
            printf("%d: Received ping\n", getpid());
        else 
        {
            printf("%d: Failed to received ping\n", getpid());
            close(fd1[0]);
            exit(0);
        }

        close(fd1[0]);

        close(fd2[0]);
        write(fd2[1], &byte_send, 1);
        close(fd2[1]);
    }
    else
    {
        close(fd1[0]);
        write(fd1[1], &byte_send, 1);
        close(fd2[1]);

        read(fd2[0], &byte_received, 1);
        if (byte_received == '.')
            printf("%d: Recieved pong\n", getpid());
        else 
            printf("%d: Failed to received pong\n", getpid());
        close(fd2[0]);

        wait(0);
    }

    return 0;
}