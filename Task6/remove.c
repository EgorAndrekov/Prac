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

int main() {
    printf("Демонтаж светофора...\n");
    
    // 1. Получаем идентификатор семафора
    int semid = semget(SEM_KEY, 2, 0666);
    if (semid == -1) {
        printf("Семафор не найден (возможно, уже удален)\n");
        return 0;
    }
    
    // 2. Устанавливаем флаг завершения для светофора
    union semun arg;
    arg.val = 0;
    
    printf("Отправка сигнала завершения светофору...\n");
    if (semctl(semid, SEM_ACTIVE, SETVAL, arg) == -1) {
        perror("Ошибка при установке флага завершения");
        // Продолжаем, возможно семафор уже удален
    }
    
    // 3. Даем светофору время на завершение
    printf("Ожидание завершения светофора (5 секунд)...\n");
    sleep(5);
    
    // 4. Теперь удаляем семафор
    printf("Удаление семафора...\n");
    if (semctl(semid, 0, IPC_RMID) == -1) {
        if (errno == EIDRM) {
            printf("Семафор уже был удален\n");
        } else {
            perror("Ошибка при удалении семафора");
        }
    } else {
        printf("Семафор удален успешно\n");
    }
    
    return 0;
}