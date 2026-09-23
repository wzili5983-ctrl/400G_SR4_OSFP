/* $Id$
 *
 *  Broadcom Confidential/Proprietary.
 *  Copyright (c) 2013 Broadcom Corporation. All rights reserved.
 */

/** @file logger.h
 * Routines for logging output to stdout and a specified file.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include "type_defns.h"

#ifndef __linux
#ifdef __cplusplus
extern "C" {
#endif
#endif

#define LOG_LEVEL_1      1
#define LOG_LEVEL_2      2

//#if !defined(__linux) || defined(LINUX_CLI)
/* Opaque SerDes Access Struct */
struct srds_access_s;
typedef struct srds_access_s srds_access_t;
//#endif

typedef struct logger_s {
    char filename[FILENAME_MAX]; ///< file name of log file.
    FILE* fp; ///< file handle.
    int verbose_level; ///< verbose level of logger.
} logger_t;

/** Open log file with the provided file name and set the verbose level.
 * @param fname 0 to disable log file or pointer to the name of the file.
 * @param overwrite_mode. If nonzero, a new log file and a backup of the
 *        exist log file of the same name will be created; otherwise,
 *        new contents will be appended to the existing log file.
 * @param verbose_level default verbose level
 * @retval 0 log file opens successfully
 * @retval 1 cannot open log file
 * @retval 2 a log file is open and in use. This happens when this function is
 *  called multiple times without calling logger_close() in between.
 */
int logger_open(logger_t *logger, int is_default, char * name);

/** Set the new verbose level for log
 * @param verbose_level new verbose level
 * @return the previous verbose level
 */
void logger_set_verbose_level(logger_t * logger, int verbose_level);

/** Return current log level.
 * @param verbose_level new verbose level
 * @return the previous verbose level
 */
int logger_get_verbose_level(logger_t * logger);

/** Write message to the logger with the designated verbose level.
 * @param message_verbose_level verbose level for the current message
 * @param format format string as in printf
 * @param ... additional variables used as in printf
 * @return the number of characters written.
 */
int logger_write(srds_access_t *sa__, int message_verbose_level, const char* format, ...);

/** Write message to the logger only with the designated verbose level.
 * @param message_verbose_level verbose level for the current message
 * @param format format string as in printf
 * @param ... additional variables used as in printf
 * @return the number of characters written.
 */
int logger_only_write(logger_t * logger, int message_verbose_level, const char* format, ...);

/** Insert a time stamp in a file along with additional information.
 * @param msg an arbitary string, usually for an action, that will be
 *  shown along with the time stamp.
 */
void logger_print_timestamp(logger_t *logger, const char* msg);

/** Close log file.
 *
 * @remarks This function is registered to atexit(). So it is called
 *  automatically upon end of program. It can be also called in the middle
 *  of a program so as to switch to another log file.
 */
void logger_close(logger_t *logger);

#if defined(__C51__) || defined(__CX51__)
#define dprintf(...) void(0)
#define ddprintf(mvl, ...) void(0)
#else
#ifdef __linux
#define dprintf printf
#else
#define dprintf(...) logger_write(0, -1, __VA_ARGS__)
#endif /*__linux*/
#define logprintf(...) logger_only_write(-1, __VA_ARGS__)
#define dprintf_time(...) logger_print_timestamp(__VA_ARGS__)
#define ddprintf(mvl, ...) logger_write(NULL, mvl, __VA_ARGS__)

#define LogMsg(format, ...)             logger_write(0, -1, format, __VA_ARGS__)
#define LogMsgLevel(level, format, ...) logger_write(0, level, format, __VA_ARGS__)
#endif

#ifndef __linux
#ifdef __cplusplus
}
#endif
#endif

#endif /* LOGGER_H_ */
