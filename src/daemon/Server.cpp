#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>
#include <map>
#include <cstring>

#include "../../headers/daemon/DaemonTools.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

std::string convertMessage(const std::string& s) {
  std::string result;

  for (const auto& c : s) {
    result.push_back(toupper(c));
  }

  return result;
}

int main(int argc, char* argv[]) {
  // Провекрка на количество параметров (вторым параметром идёт имя файла который будет использоваться).
  if (argc < 2) {
    // DEBUG
//    printf("Invalid arguments count\n");

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
    // DEBUG
//    printf("%s", ex.what());

    return -1;
  }

  // Открываем fifo в неблокирующем режиме
  char* filePath = argv[1];

  unlink(filePath);

  if (mkfifo(filePath, FILE_MODE | O_NONBLOCK) < 0) {
    // DEBUG
//    printf("Can't call mkfifo\n");

    return -1;
  }

  int inputFifoFDR;

  if ((inputFifoFDR = open(filePath, O_RDONLY)) < 0) {
    // DEBUG
//    printf("Can't open file for read\n");

    return -1;
  }

  int inputFifoFDW;

  if ((inputFifoFDW = open(filePath, O_WRONLY)) < 0) {
    // DEBUG
//    printf("Can't open file for write\n");

    return -1;
  }

  const size_t BUFFER_SIZE = 1024;
  const std::string REGISTRATION_MSG = "Knock Knock";
  const std::string DISCONNECTION_MSG = "Bye Bye";
  void* inputBuf = malloc(BUFFER_SIZE);

  std::map<pid_t, std::pair<int, int>> clients;
  clients.clear();

  std::string tmp;
  std::string msg;
  pid_t pid;

  do {
    if (read(inputFifoFDR, inputBuf, BUFFER_SIZE) > 0) {
      tmp = std::string((char*) inputBuf);
      msg = tmp.substr(0, tmp.find(':'));
      pid = std::stoi(tmp.substr(tmp.find(':') + 1, tmp.size()));


      // DEBUG
//      if (REGISTRATION_MSG == msg) {
//        printf("%s from %d\n", msg.c_str(), pid);
//      } else if (DISCONNECTION_MSG == msg) {
//        printf("Remove client %d\n", pid);
//      } else {
//        printf("%d> %s", pid, msg.c_str());
//      }
    }

    if (REGISTRATION_MSG == msg) {
      std::string path = std::to_string(pid);

      unlink(path.c_str());

      if (mkfifo(path.c_str(), FILE_MODE) < 0) {
        // DEBUG
//        printf("Can't call mkfifo\n");

        break;
      }

      int FDR;

      if ((FDR = open(path.c_str(), O_RDONLY | O_NONBLOCK)) < 0) {
        // DEBUG
//        printf("Can't open file for read\n");

        break;
      }

      int FDW;

      if ((FDW = open(path.c_str(), O_WRONLY)) < 0) {
        // DEBUG
//        printf("Can't open file for write\n");

        break;
      }

      clients.insert(std::pair<pid_t, std::pair<int, int>>(pid, std::pair<int, int>(FDR, FDW)));
    } else if (DISCONNECTION_MSG == msg) {
      // Удаление пользователя.
      std::pair<int, int> p = clients.at(pid);

      close(p.first);
      close(p.second);
      unlink(std::to_string(pid).c_str());

      clients.erase(pid);
    } else {
      // Отправляем данные пользователю.
      if (write(clients.at(pid).second, convertMessage(msg).c_str(), BUFFER_SIZE) < 0) {
        break;
      }
    }
  } while (!clients.empty());

  free(inputBuf);

  close(inputFifoFDW);
  close(inputFifoFDR);
  unlink(filePath);

  for (const auto& c : clients) {
    // Очистка ресурсов клиентов.
    close(c.second.first);
    close(c.second.second);
    unlink(std::to_string(c.first).c_str());
  }

  return 0;
}