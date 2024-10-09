#ifndef LAB1_PARENT_H
#define LAB1_PARENT_H

#include "../iolib/io.h"
#include "../errlib/errors.h"
#include "child.h"

#include <ctime>
#include <sys/wait.h>
#include <cstdlib>

error parent_process();

#endif
