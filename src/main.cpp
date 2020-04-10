#include <cstdlib>
#include <stdexcept>

#include "../headers/copy_file_test/CopyFileUtils.h"

int main(int argc, char** argv) {
  bool isCorrectArgv = cmd_utils::checkCmdArgs(argc, argv);

  if (!isCorrectArgv) {
    printf("Argv contains incorrect arguments\n");

    return EXIT_FAILURE;
  }

  int bufferSize = std::stoi(argv[1]);
  std::string inputFileName = argv[3];
  std::string outputFileName = argv[5];

  bool isSuccessCopy = copy_file_utils::copyFile(bufferSize, inputFileName, outputFileName);

  if (!isSuccessCopy) {
    printf("Copy process canceled\n");

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}