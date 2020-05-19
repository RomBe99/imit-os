#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>
#include <map>

#include "../../headers/daemon/msgdata.h"
#include "../../headers/daemon/DaemonTools.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char* argv[]) {
  // Провекрка на количество параметров (вторым параметром идёт имя файла который будет использоваться).
  if (argc < 2) {
    printf("Invalid arguments count\n");

    return -1;
  }

  // Открываем fifo в неблокирующем режиме
  char* filePath = argv[1];

  if (mkfifo(filePath, FILE_MODE) < 0) {
    printf("Can't call mkfifo\n");

    return -1;
  }

  int fdread;

  if ((fdread = open(filePath, O_RDONLY | O_NONBLOCK)) < 0) {
    printf("Can't open file for read\n");

    return -1;
  }

  int fdwrite;

  if ((fdwrite = open(filePath, O_WRONLY)) < 0) {
    printf("Can't open file for write\n");

    return -1;
  }

  char* cmd;

  if ((cmd = strrchr(argv[0], '/')) == nullptr) {
    cmd = argv[0];
  } else {
    cmd++;
  }


  // Демонизируем процесс
  try {
    demonize(cmd);
  } catch (std::runtime_error& ex) {
    printf("%s", ex.what());

    return -1;
  }

  int msgId;

  if ((msgId = msgOpen(filePath)) < 0) {
    return -1;
  }

  MSGDATA data;

  // Создадим map для клиентов и их ресурсов.
  std::map<pid_t, std::pair<int, int>> clients;

  do {
    if (msgrcv(msgId, &data, MSGDATA_LEN, 0, 0) < 0) {
      if (errno == EIDRM) {
        break;
      }

      fprintf(stderr, "Cannot receive message\n");

      break;
    }

    // Если клиент впервые попал на сервер, регистрируем его и создаём обратный fifo канал.
    if (clients.count(data.pid) == 0) {
      std::string tempName = std::to_string(data.pid);
      int fd1;
      int fd2;

      try {
        if (mkfifo(tempName.c_str(), FILE_MODE)) {
          throw std::runtime_error("Can't call mkfifo for " + tempName);
        }

        if ((fd1 = open(tempName.c_str(), O_RDONLY | O_NONBLOCK)) < 0) {
          throw std::runtime_error("Can't open file for read " + tempName);
        }

        if ((fd2 = open(tempName.c_str(), O_WRONLY)) < 0) {
          throw std::runtime_error("Can't open file for write " + tempName);
        }

        clients.insert(std::pair<pid_t, std::pair<int, int>>(data.pid, std::pair<int, int>(fd1, fd2)));
      } catch (std::runtime_error& ex) {
        printf("%s\n", ex.what());
      }
    }

    // Выводим сообщение клиенту.
    if (data.msglen > 0) {
      printf("%d> %s", data.pid, data.msgbuf);
      data.msglen = 0;
    }

    // Один из клиентов отключился.
    if (data.done == 1 && clients.count(data.pid) != 0) {
      std::pair<int, int> tmp = clients.at(data.pid);

      close(tmp.first);
      close(tmp.second);

      unlink(std::to_string(tmp.first).c_str());

      clients.erase(data.pid);
    }
  } while (!clients.empty());

  fprintf(stderr, "Server process %d exits\n", getpid());

  // Очищаем ресурсы сервера.
  msgClose(msgId);

  close(fdwrite);
  close(fdread);

  unlink(filePath);

  for (const auto& i : clients) {
    close(i.second.first);
    close(i.second.second);

    unlink(std::to_string(i.first).c_str());
  }

  return 0;
}