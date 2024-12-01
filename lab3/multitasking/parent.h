#ifndef LAB1_PARENT_H
#define LAB1_PARENT_H

#include "../iolib/io.h"
#include "../errlib/errors.h"
#include "child.h"

#include <string>
#include <ctime>
#include <sys/wait.h>
#include <cstdlib>
#include <semaphore>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

error parent_process();

#endif
