#include "../headers/file-io-tasks/file_with_hole.h"

int main(int argc, char** argv)
{
    bool flag = false;

    for (int i = 1; i < argc; ++i) {
        flag = !flag;

        if (flag) {
            ++i;
        }

        if (i % 2 == 0) {
            continue;
        }

        if (atoi(argv[i]) < 0) {
            return EXIT_FAILURE;
        }
    }

    create_file_with_hole(argc, argv);

    return EXIT_SUCCESS;
}