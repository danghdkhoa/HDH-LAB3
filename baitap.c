#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 1024

struct shared_buffer {
    int data[MAX_SIZE];
    int length;
};

int main(int argc, char *argv[]) {
    // 1. Kiểm tra input từ dòng lệnh
    if (argc != 2) {
        printf("su dung: %s so nguyen duong\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("loi vui long nhap so nguyen duong!\n");
        return 1;
    }

    // 2. Tạo vùng nhớ chia sẻ (Shared Memory)
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared_buffer), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    // Gắn vùng nhớ chia sẻ vào không gian địa chỉ của tiến trình
    struct shared_buffer *shm_ptr = (struct shared_buffer *)shmat(shmid, NULL, 0);

    pid_t pid = fork();

    if (pid == 0) {

        int index = 0;
        shm_ptr->data[index++] = n;

        while (n != 1 && index < MAX_SIZE) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            shm_ptr->data[index++] = n;
        }
        shm_ptr->length = index; // Lưu độ dài chuỗi vào shared memory


        shmdt(shm_ptr);
        exit(0);
    } 
    else {

        wait(NULL); // Chờ tiến trình con kết thúc[cite: 1]

        printf("chuoi Collatz cho n = %d la:\n", n);
        for (int i = 0; i < shm_ptr->length; i++) {
            printf("%d%s", shm_ptr->data[i], (i == shm_ptr->length - 1) ? "" : ", ");
        }
        printf("\n");


        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
