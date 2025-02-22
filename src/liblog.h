/*
 * liblog.h - Personal C99 logging library.
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

#ifndef LIBLOG_H_
#define LIBLOG_H_ 1

#include <stdbool.h>
#include <stdio.h> /* FILE */

/*----------------------------------------------------------------------------*/
/* Compile-time configuration */

/*
 * Maximum number of log files that can be registered with 'log_add_file'.
 */
#define LOG_MAX_FILES 10

/*
 * If defined, use ASCII color escape sequences when logging.
 */
#define LOG_COLOR

/*
 * Each of these macros controls the output of 'log_write'.
 */
#define LOG_DATE
#define LOG_TIME
#define LOG_TAG_NAME
#define LOG_FUNC

/*----------------------------------------------------------------------------*/
/* Log tags */

enum ELogTag {
    LOG_TAG_DBG = (1 << 0),
    LOG_TAG_INF = (1 << 1),
    LOG_TAG_WRN = (1 << 2),
    LOG_TAG_ERR = (1 << 3),
    LOG_TAG_FTL = (1 << 4),
};

/*
 * The tag with the maximum importance.
 */
#define LOG_TAG_MAX LOG_TAG_FTL

/*
 * Return an integer representing the specified tag, along with all inferior
 * ones. Useful for 'log_add_file'.
 */
#define LOG_TAG_AND_BELOW(TAG) ((TAG | (TAG - 1)))

/*
 * Return an integer representing the specified flag, along with all superior
 * ones. Useful for 'log_add_file'.
 */
#define LOG_TAG_AND_ABOVE(TAG) ((LOG_TAG_ALL & ~(TAG - 1)))

/*
 * Integer with all the tag bits set. Useful for 'log_add_file'.
 */
#define LOG_TAG_ALL LOG_TAG_AND_BELOW(LOG_TAG_MAX)

/*----------------------------------------------------------------------------*/
/* Functions and callable macros */

/*
 * Register a log file with the specified tags. Returns true if the file was
 * registered successfully.
 *
 * Whenever the 'log_write' function is called, it will write the relevant
 * information to all log files that are registered with that specific tag.
 *
 * Multiple log tags from the 'ELogTag' enum can be OR'd together for the
 * 'enabled_tags' argument.
 */
bool log_add_file(FILE* fp, enum ELogTag enabled_tags);

/*
 * Forget about all log files that were registered with 'log_add_file'.
 *
 * This function doesn't close any 'FILE', so the caller is responsible for
 * calling something like 'fclose'.  It's usually safer to call this function
 * before closing the log files.
 */
void log_clear_files(void);

/*
 * Write the specified log message to all registered log files.
 *
 * Even if the tag name is not printed, this function needs to know which logs
 * it should send the current message.
 *
 * This function is not supposed to be called directly, since the 'func'
 * argument is supposed to be set by a macro wrapper.
 */
void log_write(enum ELogTag tag, const char* func, const char* fmt, ...)
  __attribute__((format(printf, 3, 4)));

/*
 * Tag wrappers for 'log_write', meant to be called by the user.
 */
#define log_dbg(...) log_write(LOG_TAG_DBG, __func__, __VA_ARGS__)
#define log_inf(...) log_write(LOG_TAG_INF, __func__, __VA_ARGS__)
#define log_wrn(...) log_write(LOG_TAG_WRN, __func__, __VA_ARGS__)
#define log_err(...) log_write(LOG_TAG_ERR, __func__, __VA_ARGS__)
#define log_ftl(...) log_write(LOG_TAG_FTL, __func__, __VA_ARGS__)

#endif /* LIBLOG_H_ */
