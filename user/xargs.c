#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 32
#define MAXBUF 512

int main(int argc, char* argv[])
{
    if (argc - 1 >= MAXARG)
    {
        write(2, "Error: Too many arguments!", 26);
        exit(1);
    }
    
    char buffer[MAXBUF];
    char *args[MAXARG];

    for (int i = 0; i < argc - 1; i++)
        args[i] = argv[i + 1];
    
    int i = argc - 1, index = 0;

    while (index < MAXBUF && read(0, buffer + index, 1) == 1)
    {
        if (buffer[index] == '\n')
        {
            if (index == 0)
                continue;
            
            buffer[index] = '\0';
            
            if (fork() == 0)
            {
                args[i] = buffer;
                args[i + 1] = 0;
                exec(args[0], args);
                exit(0);
            }
            else wait(0);
            
            index = 0;
        }
        else index++;
    }

    exit(0);
}