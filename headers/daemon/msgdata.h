#ifndef OS_HEADERS_DAEMON_MSGDATA_H_
#define OS_HEADERS_DAEMON_MSGDATA_H_

const int BUF_LEN = 256;
const int MSGDATA_LEN = BUF_LEN + 2 * sizeof(int);
const int PERM = 0666;

typedef struct _msgdata_ {
  long int mtype;
  char msgbuf[BUF_LEN];
  int done;
  int msglen;
} MSGDATA;

int msg_open(const char* filePath);
int msg_close(int id);

#endif