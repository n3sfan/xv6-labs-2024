#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // in ra man hinh dong lenh
    if(argc == 1 || argc > 2)
    {
        char* f = "Failed to sleep command!";
        write(1, f, strlen(f));
        write(1, "\n", 1);
        exit(0);
    }
    
    char* n = "(nothing happens for a little while)";
    write(1, n, strlen(n));
    write(1, "\n", 1);
    int time = atoi(argv[1]);
    sleep(time);
    exit(0);
}