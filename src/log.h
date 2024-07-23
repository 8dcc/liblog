
#ifndef LOG_H_
#define LOG_H_ 1

#include <stdio.h>

/*----------------------------------------------------------------------------*/
/* Setting macros */

/* Default FILE* when logging */
#ifndef LOG_FP
#define LOG_FP stderr
#endif

/* NOTE: Uncomment to enable ASCII colors when logging */
#define LOG_COLOR

/*----------------------------------------------------------------------------*/
/* Enums */

enum ELogTag {
    LOG_TAG_DBG,
    LOG_TAG_INF,
    LOG_TAG_WRN,
    LOG_TAG_ERR,
    LOG_TAG_FTL,

    LOG_TAGS,
};

/*----------------------------------------------------------------------------*/
/* Functions and callable macros */

void log_write(enum ELogTag tag, const char* func, const char* fmt, ...);

#define log_dbg(...) log_write(LOG_TAG_DBG, __func__, __VA_ARGS__)
#define log_inf(...) log_write(LOG_TAG_INF, __func__, __VA_ARGS__)
#define log_wrn(...) log_write(LOG_TAG_WRN, __func__, __VA_ARGS__)
#define log_err(...) log_write(LOG_TAG_ERR, __func__, __VA_ARGS__)
#define log_ftl(...) log_write(LOG_TAG_FTL, __func__, __VA_ARGS__)

#endif /* LOG_H_ */
