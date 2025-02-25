#include "kernel/types.h"
#include "user/user.h"

#define MAX_N 280

void sieve(int lpipe[]) {
    short first_prime;
    if (read(lpipe[0], &first_prime, sizeof(short)) == 0) {
        close(lpipe[0]);
        return;
    }
    
    printf("prime %d\n", first_prime);

    int rpipe[2];
    pipe(rpipe);

    int pid = fork();
    if (pid < 0) {
        // Voi MAX_N > 280 thi fail
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // Child
        close(lpipe[0]);   
        close(rpipe[1]);    
        sieve(rpipe);      
        exit(0);
    } else {
        // Parent
        close(rpipe[0]);   
        short num;
        while (read(lpipe[0], &num, sizeof(short)) > 0) {
            if (num % first_prime != 0) {
                write(rpipe[1], &num, sizeof(short));
            }
        }
        close(lpipe[0]);  
        close(rpipe[1]);   
    }
}

int main(int argc, char *argv[]) {
    int lpipe[2];
    pipe(lpipe);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // Child: Start the sieve process
        close(lpipe[1]);    // Close unused write end
        sieve(lpipe);
        exit(0);
    } else {
        // Parent: Write numbers to the pipe
        close(lpipe[0]);     // Close unused read end
        for (int i = 2; i <= MAX_N; i++) {
            write(lpipe[1], &i, sizeof(short));
        }
        close(lpipe[1]);     // Signal EOF to sieve
        wait(0);             // Wait for sieve to finish
    }

    exit(0);
}