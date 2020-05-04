#include <dirent.h>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

using namespace std;

bool isEqualStats(struct stat s1, struct stat s2) {
  return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
}

string getCurrentPath() {
  const string ROOT_DIR = "/";
  const string CURRENT_DIR = ".";
  const string UP_DIR = "..";
  const string PATH_SEPARATOR = "/";

  struct stat rootStat{};
  struct stat currentStat{};
  struct stat tempStat{};

  string resultPath = string(ROOT_DIR);
  string currentPath = string(CURRENT_DIR);
  string upDir;
  string resPart;
  string additionalPath;

  DIR* dir;
  dirent* temp;

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

    upDir = currentPath + PATH_SEPARATOR + UP_DIR;

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw runtime_error("Can't open dir");
    }

    while ((temp = readdir(dir)) != nullptr) {
      if (CURRENT_DIR == temp->d_name || UP_DIR == temp->d_name) {
        continue;
      }

      additionalPath = upDir + PATH_SEPARATOR + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw runtime_error("Can't get info about file");
      }

      if (isEqualStats(tempStat, currentStat)) {
        resPart = string(PATH_SEPARATOR + temp->d_name);
        resPart.append(resultPath);
        resultPath = resPart;

        break;
      }
    }

    closedir(dir);

    currentPath += PATH_SEPARATOR + UP_DIR;
  } while (true);

  return resultPath;
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

  string currentPath = string(CURRENT_DIR);
  string upDir;
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

    upDir = currentPath + PATH_SEPARATOR + UP_DIR;

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw runtime_error("Can't open dir");
    }

    while ((temp = readdir(dir)) != nullptr) {
      if (CURRENT_DIR == temp->d_name || UP_DIR == temp->d_name) {
        continue;
      }

      additionalPath = upDir + PATH_SEPARATOR + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw runtime_error("Can't get info about file");
      }

      if (S_ISLNK(tempStat.st_mode)) {
        if (readlink(additionalPath.c_str(), buffer, BUFFER_SIZE) < 0) {
          throw runtime_error("Can't read content by link in " + additionalPath);
        }

        if (path == buffer) {
          count++;
        }
      }
    }

    closedir(dir);

    currentPath += PATH_SEPARATOR + UP_DIR;
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

  string currentPath = string(CURRENT_DIR);
  string upDir;
  string additionalPath;
  char* buffer = new char[BUFFER_SIZE];

  DIR* dir;
  dirent* temp;

  int count = 0;
  int i = 0;

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

    ++i;
    upDir = currentPath + PATH_SEPARATOR + UP_DIR;

    if ((dir = opendir(upDir.c_str())) == nullptr) {
      throw runtime_error("Can't open dir");
    }

    while (i % 2 == 0 && (temp = readdir(dir)) != nullptr) {
      if (CURRENT_DIR == temp->d_name || UP_DIR == temp->d_name) {
        continue;
      }

      additionalPath = upDir + PATH_SEPARATOR + temp->d_name;

      if (lstat(additionalPath.c_str(), &tempStat) < 0) {
        throw runtime_error("Can't get info about file");
      }

      if (S_ISLNK(tempStat.st_mode)) {
        if (readlink(additionalPath.c_str(), buffer, BUFFER_SIZE) < 0) {
          throw runtime_error("Can't read content by link in " + additionalPath);
        }

        count++;
      }
    }

    closedir(dir);

    currentPath += PATH_SEPARATOR + UP_DIR;
  } while (true);

  return count;
}

int main() {
  try {
    std::string absolutePath = getCurrentPath();

    int symbolicLinksCount = findAllSymbolicLinks(absolutePath);
    int metric = 1;

    printf("Count symbolic links on path %s = %d\n", absolutePath.c_str(), symbolicLinksCount);
    printf("Count symbolic links even 2 with metric %d = %d\n",
           metric, countEvenNumberSymbolicLinks(metric));
  } catch (std::runtime_error& ex) {
    printf("%s", ex.what());
  }

  return 0;
}