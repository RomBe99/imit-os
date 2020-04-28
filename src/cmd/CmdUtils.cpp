#include <dirent.h>
#include <stdexcept>
#include <utility>
#include <sys/stat.h>
#include <unistd.h>

#include "../../headers/cmd/CmdUtils.h"

DirSymbols::DirSymbols(std::string root_dir,
                       std::string current_dir,
                       std::string up_dir,
                       std::string path_separator)
    : rootDir(std::move(root_dir)),
      currentDir(std::move(current_dir)),
      upDir(std::move(up_dir)),
      pathSeparator(std::move(path_separator)) {
}

DirSymbols cmd_utils::getDirSymbolsForCurrentSystem() {
  return DirSymbols("/", ".", "..", "/");
}

bool isEqualStats(struct stat s1, struct stat s2) {
  return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
}

std::string cmd_utils::getCurrentPath() {
  const DirSymbols DIR_SYMBOLS = cmd_utils::getDirSymbolsForCurrentSystem();

  struct stat rootStat{};
  struct stat currentStat{};
  struct stat tempStat{};

  std::string resultPath = std::string(DIR_SYMBOLS.rootDir);
  std::string currentPath = std::string(DIR_SYMBOLS.currentDir);
  std::string upDir;
  std::string resPart;
  std::string additionalPath;

  DIR* dir;
  dirent* temp;

  if (lstat(DIR_SYMBOLS.rootDir.c_str(), &rootStat) < 0) {
    throw std::runtime_error("Can't get root stat");
  }

  while (!isEqualStats(rootStat, currentStat)) {
    if (lstat(currentPath.c_str(), &currentStat) < 0) {
      throw std::runtime_error("Can't get current stat");
    }

    upDir = currentPath + DIR_SYMBOLS.pathSeparator + DIR_SYMBOLS.upDir;

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw std::runtime_error("Can't open dir");
    }

    while ((temp = readdir(dir)) != nullptr) {
      if (DIR_SYMBOLS.currentDir == temp->d_name || DIR_SYMBOLS.upDir == temp->d_name) {
        continue;
      }

      additionalPath = upDir + DIR_SYMBOLS.pathSeparator + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw std::runtime_error("Can't get info about file");
      }

      if (isEqualStats(tempStat, currentStat)) {
        resPart = std::string(DIR_SYMBOLS.pathSeparator + temp->d_name);
        resPart.append(resultPath);
        resultPath = resPart;

        break;
      }
    }

    closedir(dir);

    currentPath += DIR_SYMBOLS.pathSeparator + DIR_SYMBOLS.upDir;
  }

  return resultPath;
}

int cmd_utils::findAllSymbolicLinks(const std::string& path) {
  const DirSymbols DIR_SYMBOLS = cmd_utils::getDirSymbolsForCurrentSystem();
  const int BUFFER_SIZE = 1024;

  struct stat rootStat{};
  struct stat currentStat{};
  struct stat tempStat{};

  std::string currentPath = std::string(DIR_SYMBOLS.currentDir);
  std::string upDir;
  std::string additionalPath;
  char* buffer = new char[BUFFER_SIZE];

  DIR* dir;
  dirent* temp;

  int count = 0;

  if (lstat(DIR_SYMBOLS.rootDir.c_str(), &rootStat) < 0) {
    throw std::runtime_error("Can't get root stat");
  }

  while (!isEqualStats(rootStat, currentStat)) {
    if (lstat(currentPath.c_str(), &currentStat) < 0) {
      throw std::runtime_error("Can't get current stat");
    }

    upDir = currentPath + DIR_SYMBOLS.pathSeparator + DIR_SYMBOLS.upDir;

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw std::runtime_error("Can't open dir");
    }

    while ((temp = readdir(dir)) != nullptr) {
      if (DIR_SYMBOLS.currentDir == temp->d_name || DIR_SYMBOLS.upDir == temp->d_name) {
        continue;
      }

      additionalPath = upDir + DIR_SYMBOLS.pathSeparator + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw std::runtime_error("Can't get info about file");
      }

      if (S_ISLNK(tempStat.st_mode)) {
        if (readlink(additionalPath.c_str(), buffer, BUFFER_SIZE) < 0) {
          throw std::runtime_error("Can't read content by link in " + additionalPath);
        }

        if (path == buffer) {
          count++;
        }
      }
    }

    closedir(dir);

    currentPath += DIR_SYMBOLS.pathSeparator + DIR_SYMBOLS.upDir;
  }

  return count;
}