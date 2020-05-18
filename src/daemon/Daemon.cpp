#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>
#include <stdexcept>

#include "../../headers/daemon/Daemon.h"

void demonize(const char* cmd) {
  int fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl{};
  struct sigaction sa{};

  umask(0);

  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    std::string error = ": can't get file limit";

    throw std::runtime_error(cmd + error);
  }

  if ((pid = fork()) < 0) {
    std::string error = ": can't fork";

    throw std::runtime_error(cmd + error);
  } else if (pid == 0) {
    exit(0);
  }

  setsid();

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGHUP, &sa, nullptr) < 0) {
    std::string error = ": can't ignore SIGHUP";

    throw std::runtime_error(cmd + error);
  }

  if ((pid = fork()) < 0) {
    std::string error = ": can't fork";

    throw std::runtime_error(cmd + error);
  } else if (pid == 0) {
    return;
  }

  if (chdir("/") < 0) {
    std::string error = ": can't change directory to /";

    throw std::runtime_error(cmd + error);
  }

  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }

  for (int i = 0; i < rl.rlim_max; i++) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
           fd0, fd1, fd2);
    return;
  }
}