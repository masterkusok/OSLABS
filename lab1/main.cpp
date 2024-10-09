#include "iolib/io.h"
#include "errlib/errors.h"
#include <cstdlib>
#include <sys/wait.h>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>

void invert_string(std::string* s) {
    int p1 = 0, p2 = (*s).size() - 1;
    while (p1 < p2) {
        std::swap((*s)[p1], (*s)[p2]);
        p1++;
        p2--;
    }
}

void child_process(int read_pipe_fd, const std::string& file_name) {
    int write_fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (write_fd == -1) {
        perror("Error opening file in child process");
        exit(1);
    }

    std::string buffer;
    int bytes = file_scan(read_pipe_fd, &buffer);
    while (bytes > 0 && buffer != "STOP\n") {
        buffer.erase(buffer.size() - 1); // Remove newline
        invert_string(&buffer);
        if (file_print(write_fd, buffer + "\n") == -1) {
            perror("Error writing to file");
        }
        bytes = file_scan(read_pipe_fd, &buffer);
    }

    close(read_pipe_fd);
    close(write_fd);
    exit(0);
}

void parent_process(int write_pipe_fd1, int write_pipe_fd2) {
    std::string buffer;
    int bytes = file_scan(STDIN_FILENO, &buffer);
    int target_fd;

    while (bytes > 0 && buffer != "STOP\n") {
        target_fd = (std::rand() % 100 >= 80) ? write_pipe_fd2 : write_pipe_fd1;
        if (file_print(target_fd, buffer) == -1) {
            perror("Error writing to pipe");
        }
        bytes = file_scan(STDIN_FILENO, &buffer);
    }
    if (file_print(write_pipe_fd1, "STOP\n") == -1 || file_print(write_pipe_fd2, "STOP\n") == -1) {
        perror("Error sending STOP to pipes");
    }
}

int main() {
    std::srand(std::time(nullptr));

    std::string file1_name, file2_name;

    int bytes = file_scan(STDIN_FILENO, &file1_name);
    if (bytes <= 1) {
        return ERROR_INVALID_INPUT;
    }
    file1_name.erase(file1_name.length() - 1);

    bytes = file_scan(STDIN_FILENO, &file2_name);
    if (bytes <= 1) {
        return ERROR_INVALID_INPUT;
    }
    file2_name.erase(file2_name.length() - 1);

    int pipe1_fd[2], pipe2_fd[2];
    if (pipe(pipe1_fd) == -1 || pipe(pipe2_fd) == -1) {
        perror("Error creating pipes");
        return ERROR_PIPE_FAILED;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Error forking first child");
        return ERROR_FORK_FAILED;
    }
    if (pid1 == 0) {
        close(pipe1_fd[1]);
        child_process(pipe1_fd[0], file1_name);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Error forking second child");
        return ERROR_FORK_FAILED;
    }
    if (pid2 == 0) {
        close(pipe2_fd[1]);
        child_process(pipe2_fd[0], file2_name);
    }

    close(pipe1_fd[0]);
    close(pipe2_fd[0]);

    parent_process(pipe1_fd[1], pipe2_fd[1]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    close(pipe1_fd[1]);
    close(pipe2_fd[1]);

    return STATE_OK;
}
