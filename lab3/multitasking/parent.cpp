#include <cstring>
#include "parent.h"

#define SHM_SIZE 4096
#define SEM_PARENT_READY_1 "/sem_parent_ready_1"
#define SEM_CHILD_READY_1 "/sem_child_ready_1"
#define SEM_PARENT_READY_2 "/sem_parent_ready_2"
#define SEM_CHILD_READY_2 "/sem_child_ready_2"

error parent_loop(char* shm1, char* shm2, sem_t* sem_parent_ready_1, sem_t* sem_child_ready_1,
                  sem_t* sem_parent_ready_2, sem_t* sem_child_ready_2) {
    std::string buffer;
    int bytes = std_in(&buffer);
    char* target_shm;
    sem_t* target_sem_parent_ready;
    sem_t* target_sem_child_ready;

    while (bytes > 0 && buffer != "STOP\n") {
        if (std::rand() % 100 >= 80) {
            target_shm = shm2;
            target_sem_parent_ready = sem_parent_ready_2;
            target_sem_child_ready = sem_child_ready_2;
        } else {
            target_shm = shm1;
            target_sem_parent_ready = sem_parent_ready_1;
            target_sem_child_ready = sem_child_ready_1;
        }

        sem_wait(target_sem_child_ready);
        strncpy(target_shm, buffer.c_str(), SHM_SIZE);
        sem_post(target_sem_parent_ready);

        bytes = std_in(&buffer);
    }

    sem_wait(sem_child_ready_1);
    strncpy(shm1, "STOP\n", SHM_SIZE);
    sem_post(sem_parent_ready_1);

    sem_wait(sem_child_ready_2);
    strncpy(shm2, "STOP\n", SHM_SIZE);
    sem_post(sem_parent_ready_2);

    return STATE_OK;
}

error parent_process() {
    std::srand(std::time(nullptr));

    std::string file1_name, file2_name;
    if (file_scan(STDIN_FILENO, &file1_name) <= 1 || file_scan(STDIN_FILENO, &file2_name) <= 1 ||
        file1_name == file2_name) {
        return ERROR_INVALID_INPUT;
    }
    file1_name.erase(file1_name.length() - 1);
    file2_name.erase(file2_name.length() - 1);

    // Создаём shared memory
    int shm_fd1 = shm_open("/shm1", O_CREAT | O_RDWR, 0666);
    int shm_fd2 = shm_open("/shm2", O_CREAT | O_RDWR, 0666);
    if (shm_fd1 == -1 || shm_fd2 == -1 || ftruncate(shm_fd1, SHM_SIZE) == -1 || ftruncate(shm_fd2, SHM_SIZE) == -1) {
        return ERROR_PIPE_FAILED;
    }

    char* shm1 = static_cast<char*>(mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0));
    char* shm2 = static_cast<char*>(mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0));
    if (shm1 == MAP_FAILED || shm2 == MAP_FAILED) {
        return ERROR_PIPE_FAILED;
    }

    sem_t* sem_parent_ready_1 = sem_open(SEM_PARENT_READY_1, O_CREAT, 0666, 0);
    sem_t* sem_child_ready_1 = sem_open(SEM_CHILD_READY_1, O_CREAT, 0666, 1);
    sem_t* sem_parent_ready_2 = sem_open(SEM_PARENT_READY_2, O_CREAT, 0666, 0);
    sem_t* sem_child_ready_2 = sem_open(SEM_CHILD_READY_2, O_CREAT, 0666, 1);

    if (sem_parent_ready_1 == SEM_FAILED || sem_child_ready_1 == SEM_FAILED ||
        sem_parent_ready_2 == SEM_FAILED || sem_child_ready_2 == SEM_FAILED) {
        return ERROR_PIPE_FAILED;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        child_process(shm1, sem_parent_ready_1, sem_child_ready_1, file1_name);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        child_process(shm2, sem_parent_ready_2, sem_child_ready_2, file2_name);
    }

    error err = parent_loop(shm1, shm2, sem_parent_ready_1, sem_child_ready_1, sem_parent_ready_2, sem_child_ready_2);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    munmap(shm1, SHM_SIZE);
    munmap(shm2, SHM_SIZE);
    shm_unlink("/shm1");
    shm_unlink("/shm2");
    sem_close(sem_parent_ready_1);
    sem_close(sem_child_ready_1);
    sem_close(sem_parent_ready_2);
    sem_close(sem_child_ready_2);
    sem_unlink(SEM_PARENT_READY_1);
    sem_unlink(SEM_CHILD_READY_1);
    sem_unlink(SEM_PARENT_READY_2);
    sem_unlink(SEM_CHILD_READY_2);

    return err;
}
