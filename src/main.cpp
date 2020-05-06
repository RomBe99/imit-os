#include <dirent.h>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

using namespace std;

bool isEqualStats(struct stat s1, struct stat s2) {
  return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
}

int findAllSymbolicLinks(const string& path) {
  const string ROOT_DIR = "/";
  const string CURRENT_DIR = ".";
  const string UP_DIR = "..";
  const string PATH_SEPARATOR = "/";

  const int BUFFER_SIZE = 1024;

  struct stat rootStat{};
  struct stat currentStat{};
  struct stat tempStat{};

  string currentPath = string(CURRENT_DIR + PATH_SEPARATOR);
  string upDir = currentPath;
  string additionalPath;
  char* buffer = new char[BUFFER_SIZE];

  DIR* dir;
  dirent* temp;

  int count = 0;

  if (lstat(ROOT_DIR.c_str(), &rootStat) < 0) {
    throw runtime_error("Can't get root stat");
  }

  do {
    if (lstat(currentPath.c_str(), &currentStat) < 0) {
      throw runtime_error("Can't get current stat");
    }

    if (isEqualStats(rootStat, currentStat)) {
      break;
    }

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw runtime_error("Can't open dir");
    }

    while ((temp = readdir(dir)) != nullptr) {
      if (CURRENT_DIR == temp->d_name || UP_DIR == temp->d_name) {
        continue;
      }

      additionalPath = upDir + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw runtime_error("Can't get info about file");
      }

      if (S_ISLNK(tempStat.st_mode)) {
        if (readlink(additionalPath.c_str(), buffer, BUFFER_SIZE) < 0) {
          throw runtime_error("Can't read content by link in " + additionalPath);
        }

        if (path == buffer) {
          ++count;
        }
      }
    }

    closedir(dir);

    currentPath += UP_DIR + PATH_SEPARATOR;
    upDir = currentPath + UP_DIR + PATH_SEPARATOR;
  } while (true);

  return count;
}

int countEvenNumberSymbolicLinks(const int metric) {
  if (metric < 1) {
    throw runtime_error("Entered invalid metric");
  }

  const string ROOT_DIR = "/";
  const string CURRENT_DIR = ".";
  const string UP_DIR = "..";
  const string PATH_SEPARATOR = "/";

  const int BUFFER_SIZE = 1024;

  struct stat rootStat{};
  struct stat currentStat{};
  struct stat tempStat{};

  string currentPath = string(CURRENT_DIR + PATH_SEPARATOR);
  string upDir = currentPath;
  string additionalPath;
  char* buffer = new char[BUFFER_SIZE];

  DIR* dir;
  dirent* temp;

  int count = 0;
  bool flag = false;

  if (lstat(ROOT_DIR.c_str(), &rootStat) < 0) {
    throw runtime_error("Can't get root stat");
  }

  do {
    if (lstat(currentPath.c_str(), &currentStat) < 0) {
      throw runtime_error("Can't get current stat");
    }

    if (isEqualStats(rootStat, currentStat)) {
      break;
    }

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw runtime_error("Can't open dir");
    }

    while (flag && (temp = readdir(dir)) != nullptr) {
      if (CURRENT_DIR == temp->d_name || UP_DIR == temp->d_name) {
        continue;
      }

      additionalPath = upDir + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw runtime_error("Can't get info about file");
      }

      if (S_ISLNK(tempStat.st_mode)) {
        if (readlink(additionalPath.c_str(), buffer, BUFFER_SIZE) < 0) {
          throw runtime_error("Can't read content by link in " + additionalPath);
        }

        ++count;
      }
    }

    closedir(dir);
    flag = !flag;

    currentPath += UP_DIR + PATH_SEPARATOR;
    upDir = currentPath + UP_DIR + PATH_SEPARATOR;
  } while (true);

  return count;
}

int main() {
  const int BUFFER_SIZE = 8192;
  char* buf = new char[BUFFER_SIZE];
  // Функция getwd() описана в POSIX.1-2001, но помечена как УСТАРЕВШАЯ. В POSIX.1-2008 getwd() удалена.
  // Вместо неё лучше использовать getcwd(). В POSIX.1-2001 не определены ошибки, возвращаемые getwd().
//    std::string absolutePath = std::string(getwd(buf));
  std::string absolutePath = std::string(getcwd(buf, BUFFER_SIZE));

  try {
    int symbolicLinksCount = findAllSymbolicLinks(absolutePath);
    int metric = 1;

    printf("Count symbolic links on path %s = %d\n", absolutePath.c_str(), symbolicLinksCount);
    printf("Count symbolic links even 2 with metric %d = %d\n",
           metric, countEvenNumberSymbolicLinks(metric));
  } catch (std::runtime_error& ex) {
    printf("%s", ex.what());
  }

  delete[] buf;

  return 0;
}