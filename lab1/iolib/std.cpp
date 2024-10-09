#include "std.h"

error std_in(int input_file, std::string* message) {
    char buffer[DEFAULT_BUFF_SIZE];
    std::string output;
    ssize_t bytes = read(input_file, buffer, DEFAULT_BUFF_SIZE);
    if (bytes <= 0) {
        return ERROR_CANT_WRITE_FILE;
    }
    for (int i = 0; i < bytes; i++) {
        output += buffer[i];
    }
    *message = output;
    return STATE_OK;
}

error std_out(int output_file, const std::string& message) {
    ssize_t bytes = write(output_file, message.c_str(), message.size());
    if (bytes == message.size()) {
        return STATE_OK;
    }
    return ERROR_CANT_WRITE_FILE;
}