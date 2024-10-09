#include "io.h"

int std_in(std::string* message) {
    return file_scan(STDIN_FILENO, message);
}

void std_out(const std::string& message) {
    file_print(STDOUT_FILENO, message);
}

void log_stderr(const std::string& message) {
    file_print(STDERR_FILENO, message);
}

int file_scan(int input_file, std::string* message) {
    char buffer[DEFAULT_BUFF_SIZE];
    std::string output;
    ssize_t bytes = read(input_file, buffer, DEFAULT_BUFF_SIZE);
    for (int i = 0; i < bytes; i++) {
        output += buffer[i];
    }
    *message = output;
    return (int) bytes;
}

int file_print(int output_file, const std::string& message) {
    ssize_t bytes = write(output_file, message.c_str(), message.size());
    return (int) bytes;
}