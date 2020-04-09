#ifndef OS_FILE_WITH_HOLE_H
#define OS_FILE_WITH_HOLE_H

#include "apue.h"

#define    MAXLINE    4096

static void err_doit(int errnoflag, int error, const char* fmt, va_list ap)
{
    char buf[MAXLINE];

    vsnprintf(buf, MAXLINE - 1, fmt, ap);
    if (errnoflag)
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s",
                strerror(error));
    strcat(buf, "\n");
    fflush(stdout);        /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(nullptr);        /* flushes all stdio output streams */
}

void err_sys(const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

void create_file_with_hole(size_t buf1Length, size_t buf2Length, size_t seekSize, char* buf1, char* buf2)
{
    int fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("ошибка вызова creat");
    }

    if (write(fd, buf1, buf1Length) != buf1Length) {
        err_sys("ошибка записи buf1");
    }
    /* теперь текущая позиция = buf1Length */

    if (lseek(fd, seekSize, SEEK_SET) == -1) {
        err_sys("ошибка вызова lseek");
    }
    /* теперь текущая позиция = seekSize */

    if (write(fd, buf2, buf2Length) != buf2Length) {
        err_sys("ошибка записи buf2");
    }
    /* теперь текущая позиция = seekSize + buf2Length */
}

#endif