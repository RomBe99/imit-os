#include <cerrno>
#include <cstdio>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../../headers/daemon/msgdata.h"

int msg_open(const char* filePath) {
  int msg_id;
  key_t key;

  if ((key = ftok(filePath, 'A')) < 0) {
    fprintf(stderr, "Can\'t generate key\n");

    return -1;
  }

  if ((msg_id = msgget(key, 0)) < 0) {
    if (errno == ENOENT) {
      if ((msg_id = msgget(key, IPC_CREAT | PERM)) < 0) {
        fprintf(stderr, "Cannot create message queue\n");

        return -1;
      }
    } else {
      fprintf(stderr, "Cannot get message queue\n");

      return -1;
    }
  }

  return msg_id;
}

int msg_close(int id) {
  int res;

  if ((res = msgctl(id, IPC_RMID, nullptr) < 0)) {
    fprintf(stderr, "Cannot remove message queue\n");

    return res;
  }

  return 0;
}