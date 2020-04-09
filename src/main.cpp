#include "../headers/file-io-tasks/file_with_hole.h"

int main(int argc, char** argv)
{
    // Длина первой строки
    int buf1Length = atoi(argv[1]);

    if (buf1Length < 0) {
        return EXIT_FAILURE;
    }

    // Длина второй строки
    int buf2Length = atoi(argv[2]);

    if (buf2Length < 0) {
        return EXIT_FAILURE;
    }

    // Длина "дырки"
    int seekSize = atoi(argv[3]);

    if (seekSize < 0) {
        return EXIT_FAILURE;
    }

    create_file_with_hole(buf1Length, buf2Length, seekSize, argv[4], argv[5]);

    return EXIT_SUCCESS;
}