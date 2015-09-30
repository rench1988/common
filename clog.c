/*
Copyright (c) by rench.  All Rights Reserved.
*/

#include "clog.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define LF                          (char) 10
#define CR                          (char) 13
#define CRLF                        "\x0d\x0a"
#define CLOG_LINEFEED_SIZE          1
#define CLOG_DEFAULT_ACCESS         0644
#define CLOG_INVALID_FILE           -1

#define clog_cpymem(dst, src, n)   (((char *) memcpy(dst, src, n)) + (n))
#define clog_linefeed(p)          *p++ = LF;

static char   *clog_log_errno(char *buf, char *last, int err);
static int     clog_open_file(char *filename);
static void    clog_logging_helper(int level, clog_t *clog, int err, const char *fmt, va_list  args);

static const char *err_levels[] = {
    "",
    "emerg",
    "alert",
    "crit",
    "error",
    "warn",
    "notice",
    "info",
    "debug"
};

static int clog_open_file(char *filename)
{
    if (filename == NULL) return -1;

    int fd;

    return open(filename, O_WRONLY | O_APPEND | O_CREAT, CLOG_DEFAULT_ACCESS);
}

clog_t *clog_init(char *filename, int level, int debug)
{
    char         *p, *name;
    size_t        nlen, plen;
    clog_t       *clog;

    clog = calloc(1, sizeof(clog_t));

    if (clog == NULL) {
        clog_stderr(0, "failed to calloc memory for log handler");
        goto failed;
    }

    if (!clog_level_legal(level)) {
        clog_stderr(0, "illegal level for log handler");
        goto failed;
    }

    clog->level = level;

    if (debug) {
        clog->fd = STDERR_FILENO;
    } else {
        clog->fd = clog_open_file(filename);

        if (clog->fd == CLOG_INVALID_FILE) {
            clog_stderr(errno, "failed to open file %s for log handler", filename);
            goto failed;
        }
    }

    if (filename) {
        clog_cpymem(clog->name, filename, strlen(filename));
    }

    return clog;

failed:
    if (clog) free(clog);
    return NULL;
}

static char *clog_errno(char *buf, char *last, int err)
{
    if (buf > last - 50) {
        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

    char *p = buf;

    buf = p + snprintf(buf, last - buf, " (%d: %s)", err, strerror(err));

    return buf;
}

static void clog_logging_helper(int level, clog_t *clog, int err, const char *fmt, va_list  args)
{
    char        *p, *last, *msg;
    char         errstr[CLOG_MAX_ERROR_STR];
    struct tm    tm;
    time_t       s;
    int          l;

    last = errstr + CLOG_MAX_ERROR_STR;
    s    = time(0);
    p    = errstr;

    localtime_r(&s, &tm);

    l = snprintf(errstr, sizeof(errstr), "%4d/%02d/%02d %02d:%02d:%02d",
                 tm.tm_year + 1900, tm.tm_mon + 1,
                 tm.tm_mday, tm.tm_hour,
                 tm.tm_min, tm.tm_sec);

    p = errstr + l;

    l = snprintf(p, last - p, " [%s] %d#%d: ", err_levels[level], getpid(), pthread_self());
    p = p + l;

    l = vsnprintf(p, last - p, fmt, args);
    p = p + l;

    if (err) {
        p = clog_errno(p, last, err);
    }

    if (p > last - CLOG_LINEFEED_SIZE) {
        p = last - CLOG_LINEFEED_SIZE;
    }

    clog_linefeed(p);

    (void) write(clog->fd, errstr, p - errstr);
}

void clog_stderr(int err, const char *fmt, ...)
{
    char     *p, *last;
    va_list   args;
    char      errstr[CLOG_MAX_ERROR_STR];

    last = errstr + CLOG_MAX_ERROR_STR;
    p = errstr;

    va_start(args, fmt);
    p = errstr + vsnprintf(p, CLOG_MAX_ERROR_STR, fmt, args);
    va_end(args);

    if (err) {
        p = clog_errno(p, last, err);
    }

    if (p > last - CLOG_LINEFEED_SIZE) {
        p = last - CLOG_LINEFEED_SIZE;
    }

    clog_linefeed(p);

    (void) write(STDERR_FILENO, errstr, p - errstr);
}


void clog_logging(int level, clog_t *clog, int err, const char *fmt, ...)
{
    va_list  args;

    if (clog->level >= level) {
        va_start(args, fmt);
        clog_logging_helper(level, clog, err, fmt, args);
        va_end(args);
    }
}

