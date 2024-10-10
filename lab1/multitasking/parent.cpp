#include "parent.h"

error parent_loop(int write_pipe_fd1, int write_pipe_fd2) {
    std::string buffer;
    int bytes = std_in(&buffer);
    int target_fd;

    while (bytes > 0 && buffer != "STOP\n") {
        target_fd = (std::rand() % 100 >= 80) ? write_pipe_fd2 : write_pipe_fd1;
        if (file_print(target_fd, buffer) == -1) {
            return ERROR_PIPE_FAILED;
        }
        bytes = std_in(&buffer);
    }
    if (file_print(write_pipe_fd1, "STOP\n") == -1 || file_print(write_pipe_fd2, "STOP\n") == -1) {
        return ERROR_STOP_FAILED;
    }
    return STATE_OK;
}

error parent_process() {
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

    if (file1_name == file2_name) {
        return ERROR_INVALID_INPUT;
    }

    int pipe1_fd[2], pipe2_fd[2];
    if (pipe(pipe1_fd) == -1 || pipe(pipe2_fd) == -1) {
        return ERROR_PIPE_FAILED;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        return ERROR_FORK_FAILED;
    }
    if (pid1 == 0) {
        close(pipe1_fd[1]);
        child_process(pipe1_fd[0], file1_name);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        return ERROR_FORK_FAILED;
    }
    if (pid2 == 0) {
        close(pipe2_fd[1]);
        child_process(pipe2_fd[0], file2_name);
    }

    close(pipe1_fd[0]);
    close(pipe2_fd[0]);

    error err = parent_loop(pipe1_fd[1], pipe2_fd[1]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    close(pipe1_fd[1]);
    close(pipe2_fd[1]);
    return err;
}