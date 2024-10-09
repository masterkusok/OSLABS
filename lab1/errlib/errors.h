#ifndef LAB1_ERRORS_H
#define LAB1_ERRORS_H

typedef enum {
    STATE_OK,
    ERROR_CANT_OPEN_FILE,
    ERROR_CANT_WRITE_FILE,
    ERROR_FORK_FAILED,
} error;

#endif
