#include <cstdio>
#include <csignal>

#include "../../headers/daemon/msgdata.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Invalid arguments count\n");

    return -1;
  }

  char* filePath = argv[1];

  int msgId;

  if ((msgId = msgOpen(filePath)) < 0) {
    printf("Can't get msgId\n");

    return -1;
  }

  MSGDATA data;
  data.pid = getpid();

  while (true) {
    signal(SIGHUP, SIG_IGN);

    if (!fgets(data.msgbuf, BUF_LEN, stdin)) {
      break;
    }

    data.msglen = (int) strlen(data.msgbuf) + 1;
    data.done = 0;
    data.mtype = 1;

    if (msgsnd(msgId, &data, MSGDATA_LEN, 0) < 0) {
      perror("send");
      fprintf(stderr, "Cannot send message\n");

      return -1;
    }
  }

  data.mtype = 1;
  data.done = 1;
  data.msglen = 0;

  if (msgsnd(msgId, &data, MSGDATA_LEN, 0) < 0) {
    fprintf(stderr, "Cannot send message\n");

    return -1;
  }

  fprintf(stderr, "Client process %d exits\n", getpid());

  msgClose(msgId);

  return 0;
}