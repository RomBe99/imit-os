#include <sys/msg.h>
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

#include "../../headers/daemon/Writer.h"
#include "../../headers/daemon/msgdata.h"
#include "../../headers/daemon/Daemon.h"

void writer(int msg_id) {
  MSGDATA data;

  while (true) {
    if (msgrcv(msg_id, &data, MSGDATA_LEN, 0, 0) < 0) {
      if (errno == EIDRM) {
        break;
      }

      fprintf(stderr, "Cannot receive message\n");

      return;
    }

    if (data.done) {
      break;
    }

    if (data.msglen) {
      printf("-> %s", data.msgbuf);
      data.msglen = 0;
    }
  }

  fprintf(stderr, "Writer process %d exits\n", getpid());
}

//int main(int argc, char* argv[]) {
//  const char* filePath = "writer";
//  char* cmd;
//
//  if ((cmd = strrchr(argv[0], '/')) == nullptr) {
//    cmd = argv[0];
//  } else {
//    cmd++;
//  }
//
//  try {
//    demonize(cmd);
//  } catch (std::runtime_error& ex) {
//    printf("%s", ex.what());
//  }
//
//  int msg_id;
//
//  if ((msg_id = msg_open(filePath)) < 0) {
//    return 1;
//  }
//
//  writer(msg_id);
//  msg_close(msg_id);
//
//  return 0;
//}