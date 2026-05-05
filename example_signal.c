#include <stdio.h>
#include <signal.h>

int loop_forever = 1;

void on_sigint() {
    printf("\nCRT+C is pressed!\n");
    loop_forever = 0;
}

int main() {
    loop_forever = 1;
    signal(SIGINT, on_sigint);
    while (loop_forever) {}
    return 0;
}