#include <cstdio>
#include <csignal>
#include <string>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Invalid arguments count\n");

    return -1;
  }

  char* filePath = argv[1];
  int fd;

  fd = open(filePath, O_WRONLY);

  if (fd < 0) {
    printf("Can't connect to server fifo\n");

    return -1;
  }

  pid_t pid = getpid();
  std::string serverOutput = std::to_string(pid);

  const size_t BUFFER_SIZE = 1024;

  void* serverOutputBuf = malloc(BUFFER_SIZE);
  char* serverInputBuf = new char[BUFFER_SIZE];

  std::string tmp = "Knock Knock";
  std::string postfix = ':' + std::to_string(pid);
  tmp += postfix;
  const std::string DISCONNECT_MSG = "Bye Bye";
  int serverOutputFD = -1;

  write(fd, tmp.c_str(), BUFFER_SIZE);

  while (true) {
    signal(SIGHUP, SIG_IGN);

    if (!fgets(serverInputBuf, BUFFER_SIZE, stdin)) {
      tmp = DISCONNECT_MSG + postfix;

      if (write(fd, tmp.c_str(), BUFFER_SIZE) < 0) {
        fprintf(stderr, "Can't send last client message\n");
      }

      break;
    }

    tmp = serverInputBuf + postfix;

    if (write(fd, tmp.c_str(), BUFFER_SIZE) < 0) {
      fprintf(stderr, "Can't send message to server\n");
    }

    if (serverOutputFD < 0) {
      if ((serverOutputFD = open(serverOutput.c_str(), O_RDONLY)) < 0) {
        fprintf(stderr, "Can't open server output %s\n", serverOutput.c_str());

        break;
      }
    }

    if (serverOutputFD > 0) {
      if (read(serverOutputFD, serverOutputBuf, BUFFER_SIZE) > 0) {
        printf("Server> %s", (char*) serverOutputBuf);
      }
    }
  }

  fprintf(stderr, "Client process %d exits\n", pid);

  if (fd > 0) {
    close(fd);
  }

  free(serverOutputBuf);
  delete[] serverInputBuf;

  return 0;
}