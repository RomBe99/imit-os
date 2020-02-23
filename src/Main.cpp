#include "../headers/file-io-tasks/apue.h"

#define	MAXLINE	4096

static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char	buf[MAXLINE];

    vsnprintf(buf, MAXLINE-1, fmt, ap);
    if (errnoflag)
        snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
                strerror(error));
    strcat(buf, "\n");
    fflush(stdout);		/* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(NULL);		/* flushes all stdio output streams */
}

void
err_sys(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

int main()
{
    char buf1[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";
    int fd;

    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("ошибка вызова creat");
    }

    if (write(fd, buf1, 10) != 10) {
        err_sys("ошибка записи buf1");
    }
    /* теперь текущая позиция = 10 */

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("ошибка вызова lseek");
    }
    /* теперь текущая позиция = 16384 */

    if (write(fd, buf2, 10) != 10) {
        err_sys("ошибка записи buf2");
    }
    /* теперь текущая позиция = 16394 */

    exit(0);
}