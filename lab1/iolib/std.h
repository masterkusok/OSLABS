#ifndef LAB1_STD_H
#define LAB1_STD_H

#include <fcntl.h>
#include <unistd.h>
#include <string>
#include "../errlib/errors.h"

#define DEFAULT_BUFF_SIZE 1024

error std_in(int, std::string*);
error std_out(int, const std::string&);
#endif
