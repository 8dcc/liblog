
#ifndef LOG_H_
#define LOG_H_ 1

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

/*----------------------------------------------------------------------------*/
/* Default settings */

/* Default FILE* when logging */
#ifndef LOG_FP
#define LOG_FP stderr
#endif

/*----------------------------------------------------------------------------*/
/* Tags and colors */

enum ELogTag {
    LOG_TAG_DBG,
    LOG_TAG_INF,
    LOG_TAG_WRN,
    LOG_TAG_ERR,
    LOG_TAG_FTL,

    LOG_TAGS,
};

static const char* log_tags[LOG_TAGS] = { [LOG_TAG_DBG] = "DEBUG",
                                          [LOG_TAG_INF] = "INFO ",
                                          [LOG_TAG_WRN] = "WARN ",
                                          [LOG_TAG_ERR] = "ERROR",
                                          [LOG_TAG_FTL] = "FATAL" };

#ifdef LOG_COLOR
static const char* log_colors[LOG_TAGS] = { [LOG_TAG_DBG] = "\x1b[32m",
                                            [LOG_TAG_INF] = "\x1b[36m",
                                            [LOG_TAG_WRN] = "\x1b[33m",
                                            [LOG_TAG_ERR] = "\x1b[1;31m",
                                            [LOG_TAG_FTL] = "\x1b[1;31m" };
#define LOG_COLOR_RESET "\x1b[0m"
#ifndef LOG_COLOR_DIM
#define LOG_COLOR_DIM "\x1b[37m"
#endif /* LOG_COLOR_DIM */
#endif /* LOG_COLOR */

/*----------------------------------------------------------------------------*/
/* Callable macros */

static void log_write(enum ELogTag tag, const char* func, const char* fmt,
                      ...) {
    time_t now;
    struct tm* tm;

    time(&now);
    tm = localtime(&now);

#ifdef LOG_DATE
    fprintf(LOG_FP, "%04d-%02d-%02d ", 1900 + tm->tm_year, tm->tm_mon,
            tm->tm_mday);
#endif

    fprintf(LOG_FP, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

#ifdef LOG_COLOR
    fprintf(LOG_FP, "%s %s%s%s %s%s:%s ", LOG_COLOR_RESET, log_colors[tag],
            log_tags[tag], LOG_COLOR_RESET, LOG_COLOR_DIM, func,
            LOG_COLOR_RESET);
#else
    fprintf(LOG_FP, " %s %s: ", log_tags[tag], func);
#endif

    va_list va;
    va_start(va, fmt);

    vfprintf(LOG_FP, fmt, va);
    fputc('\n', LOG_FP);
    fflush(LOG_FP);

    va_end(va);
}

#define log_dbg(...) log_write(LOG_TAG_DBG, __func__, __VA_ARGS__)
#define log_inf(...) log_write(LOG_TAG_INF, __func__, __VA_ARGS__)
#define log_wrn(...) log_write(LOG_TAG_WRN, __func__, __VA_ARGS__)
#define log_err(...) log_write(LOG_TAG_ERR, __func__, __VA_ARGS__)
#define log_ftl(...) log_write(LOG_TAG_FTL, __func__, __VA_ARGS__)

#endif /* LOG_H_ */
