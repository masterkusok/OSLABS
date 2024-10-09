#include "iolib/std.h"
#include "errlib/errors.h"

//void child_process(int read_pipe_fd, int write_fd) {
//    close(read_pipe_fd);
//    exit(0);
//}
//
//void parent_process(int write_pipe_fd1, int write_pipe_fd2) {
//    std::string buffer;
//
//    close(write_pipe_fd1);
//    close(write_pipe_fd2);
//    exit(0);
//}

int main() {
    error err;
    int input_fd = open("/dev/stdin", O_RDONLY);  // Open standard input
    int output_fd = open("/dev/stdout", O_WRONLY); // Open standard output

    if (input_fd < 0 || output_fd < 0) {
        return ERROR_CANT_OPEN_FILE;
    }

    std::string file1_name, file2_name;
    int wfd_child_1, wfd_child_2;

    err = std_in(input_fd, &file1_name);
    if (err != STATE_OK) {
        return err;
    }
    file1_name.erase(file1_name.length()-1);

    err = std_in(input_fd, &file2_name);
    if (err != STATE_OK) {
        return err;
    }
    file2_name.erase(file2_name.length()-1);

    wfd_child_1 = open(file1_name.c_str(), O_WRONLY | O_CREAT);
    wfd_child_2 = open(file2_name.c_str(), O_WRONLY | O_CREAT);
    if (wfd_child_1 < 0 || wfd_child_2 < 0) {
        return ERROR_CANT_OPEN_FILE;
    }


//    // 0 - для чтения, 1 - для записи
//    int pipe1_fd[2], pipe2_fd[2];
//    pid_t pid1, pid2;
//    pid1 = fork();
//    pid2 = fork();
//
//    if (pid1 == -1 || pid2 == -1) {
//        return ERROR_FORK_FAILED;
//    }
//
//    // для обоих дочерних процессов закрываем пайп на запись, для родительского закрываем оба пайпа на чтение
//
//    if (pid1 == 0) {
//        close(pipe1_fd[1]);
//        child_process(pipe1_fd[0]);
//    }
//    if (pid2 == 0) {
//        close(pipe2_fd[1]);
//        child_process(pipe2_fd[0]);
//    }
//
//    close(pipe1_fd[0]);
//    close(pipe2_fd[0]);
//    parent_process(pipe1_fd[1], pipe2_fd[1]);

    close(input_fd);
    close(output_fd);
    return STATE_OK;
}
