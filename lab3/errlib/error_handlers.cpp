#include "error_handlers.h"

void log_error(error err) {
    switch (err) {
        case ERROR_STOP_FAILED: {
            std_out("ERROR: CANT STOP ONE OF CHILD PROCESSES\n");
            break;
        }
        case ERROR_PIPE_FAILED: {
            std_out("ERROR: CANT CREATE PIPES\n");
            break;
        }
        case ERROR_FORK_FAILED: {
            std_out("ERROR: FORK FAILED\n");
            break;
        }
        case ERROR_INVALID_INPUT: {
            std_out("ERROR: INVALID INPUT\n");
            break;
        }
        case ERROR_CANT_WRITE_FILE: {
            std_out("ERROR: CANT WRITE FILE\n");
            break;
        }
        default: {
            std_out("UNKNOWN ERROR CODE\n");
            break;
        }
    }
}