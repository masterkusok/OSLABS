#include "multitasking/parent.h"
#include "errlib/errors.h"
#include "errlib/error_handlers.h"

int main() {
    error err = parent_process();
    if (err != STATE_OK) {
        log_error(err);
    }
    return err;
}