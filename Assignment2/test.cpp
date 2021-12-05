#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <chrono>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>

int thread_num = 1;

int main(){
    
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, sizeof(int), 0666|IPC_CREAT);

    std::cout << shmid;
    // shmat to attach to shared memory
    int* set = (int*)shmat(shmid, NULL, 0);
    std::cout << "TEST\n";
    *set = 1;
    shmdt(set);
    shmid = shmget(key, sizeof(int), 0666|IPC_CREAT);
    set = (int*)shmat(shmid, NULL, 0);
    std::cout << *set;
    shmdt(set);
    shmctl(shmid,IPC_RMID,NULL);
}