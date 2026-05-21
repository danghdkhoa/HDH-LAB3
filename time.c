#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    struct timeval start, end;
    __pid_t pid = fork();
    if (pid == 0) {
        gettimeofday(&start, NULL); // phai goi ham nay trong tien trinh con.
        
        execvp(argv[1], &argv[1]);

        perror("Error!");
        exit(1);
    }
    if (pid > 0) {
        wait(NULL);
        gettimeofday(&end, NULL);
        double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("thoi gian thuc thi: %f giay\n", time_taken);
    }


    return 0;
}