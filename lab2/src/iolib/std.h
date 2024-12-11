#ifndef SRC_STD_H
#define SRC_STD_H

#include <string>
#include <csignal>
#include <unistd.h>

#define DEFAULT_BUFF_SIZE 1024

int std_in(std::string *);

void std_out(const std::string &);

void log_stderr(const std::string &);

int file_scan(int, std::string *);

int file_print(int, const std::string &);

#endif
