#include <cstdio>
#include <cstring>
#include <sys/msg.h>
#include <unistd.h>

#include "../../headers/daemon/msgdata.h"
#include "../../headers/daemon/Reader.h"

void reader(int msg_id) {
  MSGDATA data;

  while (true) {
    if (!fgets(data.msgbuf, BUF_LEN, stdin)) {
      break;
    }

    data.msglen = (int) strlen(data.msgbuf) + 1;
    data.done = 0;
    data.mtype = 1;

    if (msgsnd(msg_id, &data, MSGDATA_LEN, 0) < 0) {
      perror("send");
      fprintf(stderr, "Cannot send message\n");

      return;
    }
  }

  data.mtype = 1;
  data.done = 1;
  data.msglen = 0;

  if (msgsnd(msg_id, &data, MSGDATA_LEN, 0) < 0) {
    fprintf(stderr, "Cannot send message\n");

    return;
  }

  fprintf(stderr, "Reader process %d exits\n", getpid());
}

//int main() {
//  const char* filePath = "writer";
//  int msg_id;
//
//  if ((msg_id = msg_open(filePath)) < 0) {
//    return 1;
//  }
//
//  reader(msg_id);
//  msg_close(msg_id);
//
//  return 0;
//}