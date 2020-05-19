#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>
#include <stdexcept>

#include "../../headers/daemon/DaemonTools.h"

void demonize(const char* cmd) {
  struct rlimit rl{};
  struct sigaction sa{};

  // Сбросим маску режима создания файла.
  umask(0);

  // Получим максимально возможный номер дескриптора файла.
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    std::string error = ": can't get file limit";

    throw std::runtime_error(cmd + error);
  }

  pid_t pid;

  // Станем лидером нового сеанса для утраты управляющего терминала.
  if ((pid = fork()) < 0) {
    std::string error = ": can't fork";

    throw std::runtime_error(cmd + error);
  } else if (pid == 0) {
    return;
  }

  setsid();

  // Запретим в будущем приобретать управляющий терминал.
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

  // Назначим корневой каталог текущим рабочим каталогом.
  if (chdir("/") < 0) {
    std::string error = ": can't change directory to /";

    throw std::runtime_error(cmd + error);
  }

  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }

  // Закроем все файловые дескрипторы.
  for (int i = 0; i < rl.rlim_max; i++) {
    close(i);
  }

  // Присоеденим файловые дескрипторы к /dev/null.
  int fd0, fd1, fd2;

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  // Начнём ведение логов.
  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
           fd0, fd1, fd2);
    return;
  }
}