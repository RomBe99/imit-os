#ifndef OS_HEADERS_COPY_FILE_TEST_COPYFILEUTILS_H_
#define OS_HEADERS_COPY_FILE_TEST_COPYFILEUTILS_H_

#include <cstdio>
#include <string>
#include <stdexcept>

#include "../cmd/CmdUtils.h"

namespace copy_file_utils {
bool copyFile(int bufferSize, const std::string& inputFileName, const std::string& outputFileName);
}

#endif