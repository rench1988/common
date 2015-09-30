/*
Copyright (c) by rench.  All Rights Reserved.
*/

#ifndef _CLOG_H_INCLUDED_
#define _CLOG_H_INCLUDED_

#define CLOG_FILE_LEN                256

#define CLOG_MAX_ERROR_STR           2048

#define CLOG_STDERR            0
#define CLOG_EMERG             1
#define CLOG_ALERT             2
#define CLOG_CRIT              3
#define CLOG_ERR               4
#define CLOG_WARN              5
#define CLOG_NOTICE            6
#define CLOG_INFO              7
#define CLOG_DEBUG             8

#define clog_emerg(log, err, fmt, ...)              clog_logging(CLOG_EMERG, log, err, fmt, __VA_ARGS__)
#define clog_alert(log, err, fmt, ...)              clog_logging(CLOG_ALERT, log, err, fmt, __VA_ARGS__)
#define clog_crit(log, err, fmt, ...)               clog_logging(CLOG_CRIT, log, err, fmt, __VA_ARGS__)
#define clog_error(log, err, fmt, ...)              clog_logging(CLOG_ERR, log, err, fmt, __VA_ARGS__)
#define clog_warn(log, err, fmt, ...)               clog_logging(CLOG_WARN, log, err, fmt, __VA_ARGS__)
#define clog_notice(log, err, fmt, ...)             clog_logging(CLOG_NOTICE, log, err, fmt, __VA_ARGS__)
#define clog_info(log, err, fmt, ...)               clog_logging(CLOG_INFO, log, err, fmt, __VA_ARGS__)
#define clog_debug(log, err, fmt, ...)              clog_logging(CLOG_DEBUG, log, err, fmt, __VA_ARGS__)

#define clog_level_legal(level)                     (level >= CLOG_EMERG && level <= CLOG_DEBUG)

#define clog_set_levels(log, level)                         \
        do {                                                \
            if (clog_level_legal(level))                    \
                log->level = level;                         \
        } while (0)

typedef struct {
    int                 level;
    int                 fd;
    char                name[CLOG_FILE_LEN];
} clog_t;

clog_t *clog_init(char *filename, int level, int debug);
void    clog_logging(int level, clog_t *clog, int err, const char *fmt, ...);
void    clog_stderr(int err, const char *fmt, ...);


#endif

