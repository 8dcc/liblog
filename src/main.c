
#include <stdio.h>
#include "liblog.h"

static void my_func(void) {
    log_dbg("Reached the desired point...");
}

int main(void) {
    log_inf("Testing formats: %d", 123);
    my_func();
    log_wrn("Got negative value");
    log_err("Unexpected value: %p", NULL);
    log_ftl("Failed to allocate enough memory");

    return 0;
}
