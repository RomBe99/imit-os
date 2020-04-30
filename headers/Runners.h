#ifndef OS_HEADERS_RUNNERS_H_
#define OS_HEADERS_RUNNERS_H_

#include <stdexcept>
#include "./cmd/CmdUtils.h"

namespace runners {
void runTask3() {
  try {
    std::string absolutePath = cmd_utils::getCurrentPath();
    int symbolicLinksCount = cmd_utils::findAllSymbolicLinks(absolutePath);
    int metric = 1;

    printf("Count symbolic links on path %s = %d\n", absolutePath.c_str(), symbolicLinksCount);
    printf("Count symbolic links even 2 with metric %d = %d\n",
        metric, cmd_utils::countEvenNumberSymbolicLinks(absolutePath, metric));
  } catch (std::runtime_error& ex) {
    printf("%s", ex.what());
  }
}
}

#endif