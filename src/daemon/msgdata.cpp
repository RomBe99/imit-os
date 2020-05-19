#include "../../headers/daemon/msgdata.h"

int msgOpen(const char* filePath) {
  int msgId;
  key_t key;

  if ((key = ftok(filePath, 'A')) < 0) {
    fprintf(stderr, "Can\'t generate key\n");

    return -1;
  }

  if ((msgId = msgget(key, 0)) < 0) {
    if (errno == ENOENT) {
      if ((msgId = msgget(key, IPC_CREAT | PERM)) < 0) {
        fprintf(stderr, "Cannot create message queue\n");

        return -1;
      }
    } else {
      fprintf(stderr, "Cannot get message queue\n");

      return -1;
    }
  }

  return msgId;
}

int msgClose(int id) {
  int res;

  if ((res = msgctl(id, IPC_RMID, nullptr) < 0)) {
    fprintf(stderr, "Cannot remove message queue\n");

    return res;
  }

  return 0;
}