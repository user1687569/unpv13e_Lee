/*
 * @Book: UNIX网络编程卷1: 套接字联网API
 * @Brief: 
 * @Date:  2022.07.14
 * @Author: user1687569
 * @Copyright (C) 2022 user1687569
 */

#include <stdarg.h>
#include <syslog.h>
#include "unp.h"


int deamon_proc;
static void err_doit(int, int, const char *, va_list);


/*
 * Nonfatal error related to system call
 * Print message and return
 */
void err_ret(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_INFO, fmt, ap);
    va_end(ap);
    return;
}


/*
 * Fatal error related to system call
 * Print message and terminates
 */
void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}


/*
 * Fatal error related to system call
 * Print message, dump core, and terminate
 */
void err_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();    /* dump core and terminate */
    exit(1);
}


/* Nonfatal error unrelated to system call
 * Print message and return 
 */
void err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}


/* Fatal error unrelated to system call
 * Print message and terminate 
 */
void err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}


/*
 * Print message and return to caller
 * Caller specifies "errnoflag" and "level"
 */
static void err_doit(int errnoflag, int level,
                     const char *fmt, va_list ap)
{
    int errno_save, n;
    char buf[MAXLINE + 1];

    errno_save = errno;
    vsprintf(buf, fmt, ap);

    n = strlen(buf);
    if(errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");

    if(deamon_proc)
    {
        syslog(level, buf);
    }
    else
    {
        fflush(stdout);
        fputs(buf, stderr);
        fflush(stderr);
    }

    return;
}

