#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    struct sysinfo info;

    if (sysinfo(&info) < 0) {
        printf("sysinfo failed\n");
        exit(1);
    }

    printf("Free memory: %ld bytes\n", info.freemem);
    printf("Running processes: %ld\n", info.nproc);

    exit(0);
}
