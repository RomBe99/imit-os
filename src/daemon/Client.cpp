#include <cstdio>
#include <csignal>
#include <string>
#include <fcntl.h>

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
  std::string serverOutput = std::to_string(data.pid);

  int fd = 0;
  bool flag = true;
  const size_t BUFFER_SIZE = 1024;

  void* buf = malloc(BUFFER_SIZE);

  while (true) {
    if (fd > 0) {
      read(fd, buf, BUFFER_SIZE);
      printf("Server> %s", (char*) buf);
    }

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

      msgClose(msgId);
      close(fd);

      return -1;
    }

    if (flag) {
      flag = !flag;

      fd = open(serverOutput.c_str(), O_RDONLY) < 0;

      if (fd < 0) {
        break;
      }
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
  close(fd);
  free(buf);

  return 0;
}