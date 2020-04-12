#include <stdexcept>
#include <unistd.h>

#include "../../headers/cmd/CmdUtils.h"
#include "../../headers/copy_file_test/CopyFileUtils.h"

bool cmd_utils::checkCmdArgs(int argc, char** argv) {
  if (argc < 2) {
    printf("Invalid parameter count\n");

    return false;
  }

  try {
    size_t bufferSize = std::stoi(argv[1]);

    if (bufferSize > 0) {
      return true;
    }
  } catch (std::invalid_argument&) {
    printf("Buffer size contain not a number\n");
  } catch (std::out_of_range&) {
    printf("Incorrect buffer size\n");
  }

  return false;
}

void copy_file_utils::copyFile(int bufferSize) {
  const int INPUT_FILE_DESCRIPTOR = 0;
  const int OUTPUT_FILE_DESCRIPTOR = 1;
  void* buffer = malloc(bufferSize);
  bool loop = true;
  int bytesRead;

  while (loop) {
    bytesRead = read(INPUT_FILE_DESCRIPTOR, buffer, bufferSize);
    loop = bytesRead > 0;

    if (loop) {
      write(OUTPUT_FILE_DESCRIPTOR, buffer, bytesRead);
    }
  }

  free(buffer);
}