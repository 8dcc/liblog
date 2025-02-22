
#include <stdio.h>
#include "liblog.h"

static void my_func(void) {
    log_dbg("Reached the desired point...");
}

int main(void) {
    FILE* err_file = fopen("error.log", "w");
    FILE* dbg_file = fopen("debug.log", "w");
    if (!err_file || !dbg_file)
        return 1;

    if (!log_add_file(stderr, LOG_TAG_ALL) ||
        !log_add_file(err_file, LOG_TAG_AND_ABOVE(LOG_TAG_ERR)) ||
        !log_add_file(dbg_file, LOG_TAG_AND_BELOW(LOG_TAG_INF))) {
        fclose(err_file);
        fclose(dbg_file);
        return 1;
    }

    log_inf("Testing formats: %d", 123);
    my_func();
    log_wrn("Got negative value");
    log_err("Unexpected value: %p", NULL);
    log_ftl("Failed to allocate enough memory");

    log_clear_files();
    fclose(err_file);
    fclose(dbg_file);

    return 0;
}
