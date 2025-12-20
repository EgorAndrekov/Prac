#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY 0xDEADBEEF
#define SEM_RED 0
#define SEM_ACTIVE 1

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int semaphore_set(int semid, int semnum, int value) {
    union semun arg;
    arg.val = value;
    if (semctl(semid, semnum, SETVAL, arg) == -1) {
        if (errno == EIDRM) {
            // Семафор был удален
            return -1;
        }
        perror("Ошибка при установке семафора");
        return -1;
    }
    return 0;
}

int main() {
    int semid = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Не удалось создать семафор");
        exit(1);
    }
    
    semaphore_set(semid, SEM_RED, 1);
    semaphore_set(semid, SEM_ACTIVE, 1);
    
    printf("Светофор запущен (PID: %d)\n", getpid());
    printf("Семафор создан с ID: %d\n", semid);
    
    while (1) {
        // Красный свет
        printf("КРАСНЫЙ свет (5 секунд)\n");
        if (semaphore_set(semid, SEM_RED, 1) == -1) {
            printf("Семафор удален, светофор завершает работу\n");
            break;
        }
        
        // Проверяем активность
        int active = semctl(semid, SEM_ACTIVE, GETVAL);
        if (active == -1) {
            if (errno == EIDRM) {
                printf("Семафор удален, светофор завершает работу\n");
                break;
            }
        } else if (active == 0) {
            printf("Светофор демонтирован (получен сигнал завершения)\n");
            break;
        }
        
        sleep(5);
        
        // Зеленый свет
        printf("ЗЕЛЕНЫЙ свет (3 секунды)\n");
        if (semaphore_set(semid, SEM_RED, 0) == -1) {
            printf("Семафор удален, светофор завершает работу\n");
            break;
        }
        
        // Проверяем активность
        active = semctl(semid, SEM_ACTIVE, GETVAL);
        if (active == -1) {
            if (errno == EIDRM) {
                printf("Семафор удален, светофор завершает работу\n");
                break;
            }
        } else if (active == 0) {
            printf("Светофор демонтирован (получен сигнал завершения)\n");
            break;
        }
        
        sleep(3);
    }
    
    return 0;
}