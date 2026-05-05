#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 10


struct shared_data {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    int sum;
    int stop; 
};

int main() {

    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared_data), IPC_CREAT | 0666);
    struct shared_data *data = (struct shared_data *)shmat(shmid, NULL, 0);

    data->in = 0;
    data->out = 0;
    data->count = 0;
    data->sum = 0;
    data->stop = 0;

    pid_t pid = fork();

    if (pid == 0) {
        // CONSUMER 
        while (1) {
            // Đợi nếu buffer trống và chưa có lệnh dừng
            while (data->count == 0 && data->stop == 0);

            if (data->stop == 1 && data->count == 0) break;


            int item = data->buffer[data->out];
            data->sum += item;
            data->count--;
            
            printf("CONSUMER | Read: %d | Total Sum: %d | Buffer count: %d\n", item, data->sum, data->count);
            
            data->out = (data->out + 1) % BUFFER_SIZE;

            // Kiểm tra điều kiện dừng
            if (data->sum > 100) {
                printf("CONSUMER | Sum > 100. Stopping...\n");
                data->stop = 1;
                break;
            }
            usleep(200000); // Ngủ 0.2s để dễ quan sát
        }
        shmdt(data);
        exit(0);
    } else {
        // PRODUCER
        srand(time(NULL));
        while (1) {

            while (data->count == BUFFER_SIZE && data->stop == 0);

            if (data->stop == 1) break;

            // Tạo số ngẫu nhiên [10, 20]
            int nextProduce = (rand() % 11) + 10;
            
            data->buffer[data->in] = nextProduce;
            data->count++;
            
            printf("PRODUCER | Produced: %d | Buffer count: %d\n", nextProduce, data->count);
            
            data->in = (data->in + 1) % BUFFER_SIZE;

            usleep(150000); // Ngủ 0.15s
        }

        // Đợi con kết thúc
        wait(NULL);

        // Giải phóng Shared Memory
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Main process finished. Shared memory cleaned.\n");
    }

    return 0;
}