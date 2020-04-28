#ifndef OS_HEADERS_RUNNERS_H_
#define OS_HEADERS_RUNNERS_H_

#include <stdexcept>
#include "./cmd/CmdUtils.h"

namespace runners {
void runTask3() {
  try {
    std::string absolutePath = cmd_utils::getCurrentPath();
    int symbolicLinksCount = cmd_utils::findAllSymbolicLinks(absolutePath);

    printf("Count symbolic links on path %s = %d\n", absolutePath.c_str(), symbolicLinksCount);
  } catch (std::runtime_error& ex) {
    printf("%s", ex.what());
  }
}
}

#endif