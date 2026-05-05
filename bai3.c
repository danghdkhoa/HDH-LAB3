#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
pid_t child_pid = -1;
int press_count = 0;
void handle_sigint(int sig) {
    press_count++;
    
    if (press_count == 1) {
        if (child_pid > 0) {
            
            kill(child_pid, SIGKILL); 
            printf("\n./count.sh has stopped\n");
        }
    } else {

        printf("\nExiting main program...\n");
        exit(0);
    }
}
int main() {
    printf("Welcome to IT007, I am 24520809!\n");

    signal(SIGINT, handle_sigint);
    child_pid = fork();

    if (child_pid == 0) {

        setpgid(0, 0); 
        
        execl("./count.sh", "count.sh", "120", (char *)NULL);
        perror("execl failed");
        exit(1);
    } else {

        wait(NULL);

        while (1) {
            pause(); // sleep
        }
    }
    return 0;
}