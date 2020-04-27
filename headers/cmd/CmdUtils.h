#ifndef OS_HEADERS_CMD_CMDUTILS_H_
#define OS_HEADERS_CMD_CMDUTILS_H_

#include <string>

struct DirSymbols {
  std::string rootDir;
  std::string currentDir;
  std::string upDir;
  std::string pathSeparator;
};

namespace cmd_utils {
bool checkCmdArgs(int argc, char** argv);
DirSymbols getDirSymbolsForCurrentSystem();
std::string getCurrentPath();
}

#endif