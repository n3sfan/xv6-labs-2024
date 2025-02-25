#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: touch filename\n");
        exit(1);
    }

    int fd = open(argv[1], O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf("touch: cannot create %s\n", argv[1]);
        exit(1);
    }
    close(fd);
    exit(0);
}
