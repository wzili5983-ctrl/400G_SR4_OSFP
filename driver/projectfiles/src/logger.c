/* $Id$
 *
 *  Broadcom Confidential/Proprietary.
 *  Copyright (c) 2013 Broadcom Corporation. All rights reserved.
 */

/** @file logger.c
 *  Implementation of logger utility.
 */

#if (defined(__linux__) || defined(_WIN32)) && !defined(BCM_ARM_LOGGER_TEST)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifndef __linux 
#include <direct.h>
#endif

#include <time.h>
#include <sys/stat.h>
#include "logger.h"

#ifndef __linux
#define stat_get           _stat
#define make_dir(x, y)     _mkdir(x)
#else
#define stat_get           stat
#define make_dir(x, y)     mkdir(x, y)
#endif

extern int logger_write_for_estq_dll(int message_verbose_level, char* buff);

logger_t * default_logger = NULL;

/** Insert a time stamp in a file along with additional information.
 * @param fp the file pointer for the file to write, assuming to be not NULL.
 * @param info_str additional information to print, usually 'start' or 'end',
 *        along with the time stamp.
 *
 * @remarks This function could have been unnecessary if ctime() does not
 *  automatically adds a '\n' at the end of its output.
 */
static void logger_timestamp(FILE* fp, const char* info_str)
{
    char time_str[32];
    time_t curr_time;
    struct tm * tm_ptr;

    time(&curr_time);
    tm_ptr = localtime(&curr_time);

    sprintf(time_str, "%d-%d-%d %d:%d:%d", tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday, 
                                                               tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);

    fprintf(fp, "\n======== LOG %s on %s ========\n", info_str, time_str);
}

int logger_open(logger_t *logger, int is_default, char * name)
{
    struct tm * tm_ptr;
    char time_str[32];
    char * file_ext = ".log";
    time_t curr_time;

#ifndef __linux
    struct _stat file_stat = {0};
#else
    struct stat file_stat = {0};
#endif

    if (logger->fp)
    {
        // file pointer already initialized
        // To do: return meaningful error code
        return 2;
    }

    if (!strlen(logger->filename)) {
        // invalid file name
        // To do: return meaningful error code
        return 3;
    }

    stat_get(logger->filename, &file_stat);

    if ((file_stat.st_mode & S_IFDIR) == 0) {
        make_dir(logger->filename, 0777);
    }

    strncat(logger->filename, "/", sizeof(logger->filename) - strlen(logger->filename) - 1);
    strncat(logger->filename, name, sizeof(logger->filename) - strlen(logger->filename) - 1);

    memset(time_str, 0, sizeof(time_str));
    time(&curr_time);
    tm_ptr = localtime(&curr_time);
    if (is_default) {
        sprintf(time_str, "_%d%d%dT%d%d%d", tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday, 
                                                               tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    }
    else {
        memset(time_str, 0, sizeof(time_str));
        file_ext = "";
    }

    if(FILENAME_MAX <= (strlen(logger->filename) + strlen(time_str) + strlen(file_ext))) {
        printf("filename %s is too long, will be truncated\n", logger->filename);
        strncat(&(logger->filename[FILENAME_MAX - (strlen(time_str) + strlen(file_ext) + 2)]), time_str, strlen(time_str));
        strncat(&(logger->filename[FILENAME_MAX - (strlen(file_ext) + 2)]), time_str, strlen(file_ext));
    }
    else {
        strncat(logger->filename, time_str, strlen(time_str));
        strncat(logger->filename, file_ext, strlen(file_ext));
    }
    logger->filename[FILENAME_MAX - 1] = 0;

    if ((logger->fp = fopen(logger->filename, "wt")) != NULL)
    {
        default_logger = logger;
        if (is_default) {
            logger_timestamp(logger->fp, "starts");
            fprintf(logger->fp, "\n");
        }
        return 0;
    }
    else
    {
        // Fail to allocate a file pointer
        // To do: return meaningful error code
        return 4;
    }
}

void logger_set_verbose_level(logger_t * logger, int verbose_level)
{
    logger->verbose_level = verbose_level;
}

int logger_get_verbose_level(logger_t * logger)
{
    return logger->verbose_level;
}

int logger_write(srds_access_t *sa__, int message_verbose_level, const char* format, ...)
{
    int n = 0;
    static char buf[4096] = {0};

    if (default_logger && message_verbose_level <= default_logger->verbose_level) {
        va_list args;
        va_start(args, format);

        if (default_logger->fp) {
            n = vsprintf(buf, format, args);
            fprintf(default_logger->fp, "%s", buf);
            printf("%s", buf);
        } else {
            n = vprintf(format, args);
        }
        va_end(args);
    }

    return n;
}

void logger_close(logger_t *logger)
{
    if (logger && logger->fp) {
        if (default_logger == logger) {
            default_logger = NULL;
        }
        fclose(logger->fp);
        memset(logger, 0, sizeof(logger_t));
    }
}

void logger_print_timestamp(logger_t *logger, const char* msg)
{
    if (logger && logger->fp) {
        logger_timestamp(logger->fp, msg);
    }
}

int logger_only_write(logger_t * logger, int message_verbose_level, const char* format, ...)
{
    int n = 0;
    static char buf[4096] = { 0 };

    if (message_verbose_level <= logger->verbose_level) {
        va_list args;
        va_start(args, format);

        if (logger && logger->fp) {
            n = vsprintf(buf, format, args);
            fprintf(logger->fp, "%s", buf);
            //printf("%s", buf);
        } /*else {
            n = vprintf(format, args);
        }*/
        va_end(args);
    }

    return n;
}
#endif


#if (!defined(__linux__) && !defined(_WIN32)) || defined(BCM_ARM_LOGGER_TEST)
#include "logger.h"
#include "bcm_arm_platform.h"
#include <stdarg.h>
#include <string.h>
char bcm_arm_log[1024];
volatile uint32_t bcm_arm_log_written;
static logger_t *default_logger;
static int arm_log(logger_t *logger, int level, const char *format, va_list args)
{
    char buffer[256];
    int length;
    size_t i, count;
    if (format == NULL) { return -1; }
    if (logger != NULL && level > logger->verbose_level) { return 0; }
    length = vsnprintf(buffer, sizeof(buffer), format, args);
    if (length < 0) { return length; }
    count = (size_t)length < sizeof(buffer) ? (size_t)length : sizeof(buffer) - 1;
    for (i = 0; i < count; ++i) {
        bcm_arm_log[bcm_arm_log_written % sizeof(bcm_arm_log)] = buffer[i];
        ++bcm_arm_log_written;
    }
    return (int)count;
}
int logger_open(logger_t *logger, int is_default, char *name)
{
    if (logger == NULL) { return 1; }
    memset(logger, 0, sizeof(*logger));
    if (name != NULL) { snprintf(logger->filename, sizeof(logger->filename), "%s", name); }
    logger->verbose_level = LOG_LEVEL_2;
    if (is_default) { default_logger = logger; }
    return 0;
}
void logger_set_verbose_level(logger_t *logger, int level) { if (logger != NULL) { logger->verbose_level = level; } }
int logger_get_verbose_level(logger_t *logger) { return logger != NULL ? logger->verbose_level : LOG_LEVEL_2; }
int logger_write(srds_access_t *sa__, int level, const char *format, ...)
{
    int result; va_list args; (void)sa__;
    va_start(args, format); result = arm_log(default_logger, level, format, args); va_end(args);
    return result;
}
int logger_only_write(logger_t *logger, int level, const char *format, ...)
{
    int result; va_list args;
    va_start(args, format); result = arm_log(logger, level, format, args); va_end(args);
    return result;
}
void logger_print_timestamp(logger_t *logger, const char *msg)
{
    logger_only_write(logger, -1, "[log %lu] %s\n", (unsigned long)bcm_arm_log_written, msg != NULL ? msg : "");
}
void logger_close(logger_t *logger) { if (default_logger == logger) { default_logger = NULL; } }
#endif
