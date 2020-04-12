#include <cstdio>
#include <cstdlib>
#include <string>

#include "../headers/copy_file_test/CopyFileUtils.h"

int main(int argc, char** argv) {
  bool isCorrectArgv = cmd_utils::checkCmdArgs(argc, argv);

  if (!isCorrectArgv) {
    printf("Argv contains incorrect arguments\n");

    return EXIT_FAILURE;
  }

  int bufferSize = std::stoi(argv[1]);

  bool isSuccessCopy = copy_file_utils::copyFile(bufferSize);

  if (!isSuccessCopy) {
    printf("Copy process canceled\n");

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}