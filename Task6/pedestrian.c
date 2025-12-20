#define _GNU_SOURCE  // Для semtimedop
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY 0xDEADBEEF
#define SEM_RED 0
#define SEM_ACTIVE 1

#define MSG_WAITING "Ждет зеленого сигнала"
#define MSG_CROSSING "Переходит дорогу по переходу"
#define MSG_WALKING "Гуляет"

int check_semaphore_active(int semid) {
    int active = semctl(semid, SEM_ACTIVE, GETVAL);
    if (active == -1) {
        if (errno == EIDRM) {
            return -1; // Семафор удален
        }
    }
    return active;
}

int main(int argc, char *argv[]) {
    
    int walk_interval = atoi(argv[1]);
    
    pid_t pid = getpid();
    
    // Получаем идентификатор семафора
    int semid = semget(SEM_KEY, 2, 0666);
    
    printf("Пешеход %d запущен, гуляет каждые %d секунд\n", pid, walk_interval);
    
    while (1) {
        // Гуляем
        int walk_time = walk_interval;
        sleep(walk_time);
        
        fprintf(stderr, "Пешеход %d: %s\n", pid, MSG_WALKING);
        
        // Подходим к переходу
        fprintf(stderr, "Пешеход %d: %s\n", pid, MSG_WAITING);
        
        // Проверяем, активен ли светофор
        if (check_semaphore_active(semid) != 1) {
            fprintf(stderr, "Пешеход %d: светофор демонтирован, завершение\n", pid);
            break;
        }
        
        // Ждем зеленого света
        struct sembuf wait_green = {SEM_RED, 0, 0};
        
        while (1) {
            // Пытаемся перейти (ждем зеленого с таймаутом 1 сек)
            struct timespec timeout;
            timeout.tv_sec = 1;
            timeout.tv_nsec = 0;
            
            if (semtimedop(semid, &wait_green, 1, &timeout) == 0) {
                // Зеленый свет
                fprintf(stderr, "Пешеход %d: %s\n", pid, MSG_CROSSING);
                sleep(1);
                fprintf(stderr, "Пешеход %d: перешел дорогу\n", pid);
                break;
            } else {
                // Таймаут или ошибка
                if (errno == EIDRM || errno == EINVAL) {
                    // Семафор удален
                    fprintf(stderr, "Пешеход %d: светофор демонтирован, завершение\n", pid);
                    return 0;
                }
                
                // Проверяем активность светофора
                if (check_semaphore_active(semid) != 1) {
                    fprintf(stderr, "Пешеход %d: светофор демонтирован, завершение\n", pid);
                    return 0;
                }
            }
        }
    }
    
    return 0;
}