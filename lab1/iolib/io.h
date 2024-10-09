#ifndef LAB1_IO_H
#define LAB1_IO_H

#include <fcntl.h>
#include <unistd.h>
#include <string>

#define DEFAULT_BUFF_SIZE 1024

void log_stderr(const std::string&);
int std_in(std::string*);
void std_out(const std::string&);
int file_scan(int input_file, std::string *message);
int file_print(int output_file, const std::string &message);
#endif
