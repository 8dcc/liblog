/*
 * liblog.c - Personal C99 logging library.
 * See: https://github.com/8dcc/liblog
 * Copyright (C) 2024 8dcc
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

/* NOTE: Remember to change the path if you move the header */
#include "liblog.h"

/*----------------------------------------------------------------------------*/
/* Private macros */

#define IGNORE_UNUSED(VAR) (void)(VAR)

/*----------------------------------------------------------------------------*/
/* Private structures */

/*
 * Structure representing a log file, long with the OR'd tags that should be
 * written to it.
 */
struct LogFile {
    FILE* fp;
    enum ELogTag tags;
};

/*----------------------------------------------------------------------------*/
/* Private variables */

/*
 * Internal list of log files, and the current position in the list.
 */
static struct LogFile g_log_files[LOG_MAX_FILES];
static size_t g_log_file_num = 0;

/*
 * List of tag names, used when 'LOG_TAG_NAME' is defined.
 */
static const char* g_log_tag_names[] = { [LOG_TAG_DBG] = "DEBUG",
                                         [LOG_TAG_INF] = "INFO ",
                                         [LOG_TAG_WRN] = "WARN ",
                                         [LOG_TAG_ERR] = "ERROR",
                                         [LOG_TAG_FTL] = "FATAL" };

#ifdef LOG_COLOR
#define LOG_COLOR_RESET "\x1b[0m"
#ifndef LOG_COLOR_DIM
#define LOG_COLOR_DIM "\x1b[37m"
#endif /* LOG_COLOR_DIM */
static const char* g_log_colors[] = { [LOG_TAG_DBG] = "\x1b[32m",
                                      [LOG_TAG_INF] = "\x1b[36m",
                                      [LOG_TAG_WRN] = "\x1b[33m",
                                      [LOG_TAG_ERR] = "\x1b[1;31m",
                                      [LOG_TAG_FTL] = "\x1b[1;31m" };
#else /* !LOG_COLOR */
#define LOG_COLOR_RESET ""
#define LOG_COLOR_DIM   ""
static const char* g_log_colors[] = { [LOG_TAG_DBG] = "",
                                      [LOG_TAG_INF] = "",
                                      [LOG_TAG_WRN] = "",
                                      [LOG_TAG_ERR] = "",
                                      [LOG_TAG_FTL] = "" };
#endif /* LOG_COLOR */

/*----------------------------------------------------------------------------*/
/* Private functions */

static void log_write_fp(FILE* fp, enum ELogTag tag, const char* func,
                         const char* fmt, va_list va) {
    time_t now;
    struct tm* tm;

    /* Avoid -Wunused-variable */
    IGNORE_UNUSED(g_log_tag_names);
    IGNORE_UNUSED(g_log_colors);
    IGNORE_UNUSED(tag);
    IGNORE_UNUSED(func);
    IGNORE_UNUSED(now);
    IGNORE_UNUSED(tm);

    time(&now);
    tm = localtime(&now);

#ifdef LOG_DATE
    /* Draw the date */
    fprintf(fp, "%04d-%02d-%02d ", 1900 + tm->tm_year, tm->tm_mon, tm->tm_mday);
#endif

#ifdef LOG_TIME
    /* Draw the time */
    fprintf(fp, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
#endif

#ifdef LOG_TAG_NAME
    /* Draw the tag name (ERROR, WARNING, etc.) */
    fprintf(fp,
            "%s%s%s ",
            g_log_colors[tag],
            g_log_tag_names[tag],
            LOG_COLOR_RESET);
#endif

#ifdef LOG_FUNC
    /* Draw the function name */
    fprintf(fp, "%s%s:%s ", LOG_COLOR_DIM, func, LOG_COLOR_RESET);
#endif

    /* Draw the user message */
    vfprintf(fp, fmt, va);
    fputc('\n', fp);
    fflush(fp);
}

/*----------------------------------------------------------------------------*/
/* Public functions */

bool log_add_file(FILE* fp, enum ELogTag enabled_tags) {
    if (g_log_file_num >= LOG_MAX_FILES)
        return false;

    g_log_files[g_log_file_num].fp   = fp;
    g_log_files[g_log_file_num].tags = enabled_tags;
    g_log_file_num++;

    return true;
}

void log_clear_files(void) {
    g_log_file_num = 0;
}

void log_write(enum ELogTag tag, const char* func, const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    for (size_t i = 0; i < g_log_file_num; i++) {
        const struct LogFile log_file = g_log_files[i];
        if ((log_file.tags & tag) == 0)
            continue;

        /*
         * TODO: Specify whether we want to print colors or different components
         * depending on the file.
         */
        log_write_fp(log_file.fp, tag, func, fmt, va);
    }

    va_end(va);
}
