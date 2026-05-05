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

    // read from the shared memory object
    printf("Read shared memory: ");
    printf("%s\n", (char*)ptr);

    // update the shared memory object
    strcpy(ptr, "Hello Process A");
    printf("Shared memory updated: %s\n", ptr);
    sleep(5);


    munmap(ptr, SIZE); // unmap the shared memory segment and close the file descriptor
    close(fd);

    shm_unlink(name); // remote the shared memory segment
    return 0;
}