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

void create_file_with_hole(int argc, char** argv)
{
    int fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("ошибка вызова creat");
    }

    int length;
    int temp = 0;
    bool flag = true;

    for (int i = 1; i < argc; flag = !flag) {
        if (flag) {
            length = atoi(argv[i]);
            temp += length;
            ++i;

            if (write(fd, argv[i], length) != length) {
                err_sys("ошибка записи");
            }

            ++i;
        }
        else {
            temp += atoi(argv[i]);
            if (lseek(fd, temp, SEEK_SET) == -1) {
                err_sys("ошибка вызова lseek");
            }

            ++i;
        }
    }
}

#endif