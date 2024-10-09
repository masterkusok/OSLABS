#include "child.h"

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
        log_stderr("Error opening file in child process");
        exit(1);
    }

    std::string buffer;
    int bytes = file_scan(read_pipe_fd, &buffer);
    while (bytes > 0 && buffer != "STOP\n") {
        buffer.erase(buffer.size() - 1);
        invert_string(&buffer);
        if (file_print(write_fd, buffer + "\n") == -1) {
            log_stderr("Error writing to file");
            exit(1);
        }
        bytes = file_scan(read_pipe_fd, &buffer);
    }

    close(read_pipe_fd);
    close(write_fd);
    exit(0);
}