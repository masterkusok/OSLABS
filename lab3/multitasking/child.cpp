#include "child.h"

void invert_string(std::string* s) {
    int p1 = 0, p2 = (*s).size() - 1;
    while (p1 < p2) {
        std::swap((*s)[p1], (*s)[p2]);
        p1++;
        p2--;
    }
}

sem_t child_process(char* shm, sem_t* sem_parent_ready, sem_t* sem_child_ready, const std::string& file_name) {
    int write_fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (write_fd == -1) {
        log_stderr("Error opening file in child process");
        exit(1);
    }

    std::string buffer;

    while (true) {
        sem_wait(sem_parent_ready);
        buffer = std::string(shm);

        if (buffer == "STOP\n") {
            break;
        }

        buffer.erase(buffer.size() - 1);
        invert_string(&buffer);

        if (file_print(write_fd, buffer + "\n") == -1) {
            log_stderr("Error writing to file");
            exit(1);
        }

        sem_post(sem_child_ready);
    }

    close(write_fd);
    exit(0);
}
