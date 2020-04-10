#include "../../headers/copy_file_test/CopyFileUtils.h"

bool cmd_utils::checkCmdArgs(int argc, char** argv) {
  const int ARGUMENTS_COUNT = 6;
  const int MIN_BUFFER_SIZE = 0;
  const int MAX_BUFFER_SIZE = 128;
  const std::string INPUT_OPERATOR = "<";
  const std::string OUTPUT_OPERATOR = ">";

  if (argc != ARGUMENTS_COUNT) {
    printf("Invalid parameter count\n");

    return false;
  }

  try {
    int bufferSize = std::stoi(argv[1]);

    if (bufferSize < MIN_BUFFER_SIZE || bufferSize > MAX_BUFFER_SIZE) {
      throw std::out_of_range("");
    }
  } catch (std::invalid_argument&) {
    printf("Buffer size contain not a number\n");

    return false;
  } catch (std::out_of_range&) {
    printf("Incorrect buffer size\n");

    return false;
  }

  if (std::string(argv[2]) != INPUT_OPERATOR) {
    printf("Invalid input char\n");

    return false;
  }

  if (std::string(argv[4]) != OUTPUT_OPERATOR) {
    printf("Invalid output operator\n");

    return false;
  }

  return true;
}

// TODO Убрать заглушку
bool copy_file_utils::copyFile(int bufferSize, const std::string& inputFileName, const std::string& outputFileName) {
  return true;
}