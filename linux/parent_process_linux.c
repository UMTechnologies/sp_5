#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int checkFileContent(const char* filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return -1;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == 0) {
        fprintf(stderr, "Error: File %s is empty.\n", filePath);
        close(fd);
        return -2;
    }
    close(fd);

    FILE* file = fopen(filePath, "r");
    double number;
    int validNumbers = 0;
    while (fscanf(file, "%lf", &number) == 1) {
        validNumbers++;
    }
    fclose(file);

    if (validNumbers == 0) {
        fprintf(stderr, "Error: No valid numbers found in file %s.\n", filePath);
        return -3;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_children> <path_to_numbers_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (checkFileContent(argv[2]) != 0) {
        exit(EXIT_FAILURE);
    }

    int nChildren = atoi(argv[1]);
    if (nChildren <= 0) {
        fprintf(stderr, "Error: The number of children must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nChildren; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./child_process_linux", "child_process_linux", argv[2], NULL);
            fprintf(stderr, "Error executing child process\n");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        }
    }

    while (nChildren > 0) {
        wait(NULL);
        nChildren--;
    }

    printf("Total sum of squares: calculation completed by child processes.\n");
    return 0;
}
