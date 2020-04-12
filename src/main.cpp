#include <cstdio>
#include <cstdlib>
#include <string>

#include "../headers/cmd/CmdUtils.h"
#include "../headers/copy_file_test/CopyFileUtils.h"

int main(int argc, char** argv) {
  bool isCorrectArgv = cmd_utils::checkCmdArgs(argc, argv);

  if (!isCorrectArgv) {
    printf("Argv contains incorrect arguments\n");

    return EXIT_FAILURE;
  }

  int bufferSize = std::stoi(argv[1]);

  copy_file_utils::copyFile(bufferSize);

  return EXIT_SUCCESS;
}