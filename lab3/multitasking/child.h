#ifndef LAB1_CHILD_H
#define LAB1_CHILD_H

#include <string>
#include <semaphore>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include "../iolib/io.h"

sem_t child_process(char *, sem_t *, sem_t *, const std::string &);

#endif
