#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

int main() {
    const int SIZE = 4096; // the size (in bytes) of shared memory object
    const char *name = "OS"; // name of the shared memory object
    int fd; // shared memory file descriptor
    char *ptr; // pointer to shared memory object
    
    fd = shm_open(name, O_CREAT | O_RDWR, 0666); // create the shared memory object

    ftruncate(fd, SIZE); // configure the size of the shared memory object

    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // memory map the shared memory object

    strcpy(ptr, "Hello Process B"); // write to the shared memory object

    // wait until Process B updates the shared memory segment
    while (strncmp(ptr, "Hello Process B", 15) == 0) {
        printf("Waiting Process B update shared memory\n");
        sleep(1);
    }
    printf("Memory updated: %s\n", (char*)ptr);

    munmap(ptr, SIZE); // unmap the shared memory segment and close the file descriptor

    close(fd);
    return 0;

}