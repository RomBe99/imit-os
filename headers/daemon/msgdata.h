#ifndef OS_HEADERS_DAEMON_MSGDATA_H_
#define OS_HEADERS_DAEMON_MSGDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

const int BUF_LEN = 256;
const int MSGDATA_LEN = BUF_LEN + 2 * sizeof(int) + sizeof(pid_t);
const int PERM = 0666;

typedef struct _msgdata_ {
  long int mtype;
  char msgbuf[BUF_LEN];
  int done;
  int msglen;
  pid_t pid;
} MSGDATA;

int msgOpen(const char* filePath);
int msgClose(int id);

#endif