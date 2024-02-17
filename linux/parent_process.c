#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nChildren = atoi(argv[1]);
    int totalNumbers = 10; // Assuming we know the total numbers in advance.

    if (nChildren > totalNumbers / 2) {
        nChildren = totalNumbers / 2;
        printf("Number of children adjusted to %d\n", nChildren);
    }

    int numbersPerChild = totalNumbers / nChildren;
    int extraNumbers = totalNumbers % nChildren;
    pid_t pids[nChildren];

    for (int i = 0; i < nChildren; i++) {
        int start = i * numbersPerChild;
        int end = (i + 1) * numbersPerChild + (i == nChildren - 1 ? extraNumbers : 0);

        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) { // Child process
            char cmd[260];
            sprintf(cmd, "./child_process %d %d %d", start, end, i);
            execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
            // If execl returns, there was an error
            perror("execl failed");
            exit(EXIT_FAILURE);
        }
    }

    double totalSum = 0;
    for (int i = 0; i < nChildren; i++) {
        waitpid(pids[i], NULL, 0); // Wait for each child to complete

        char resultFilename[BUFFER_SIZE];
        sprintf(resultFilename, "result_%d.txt", i);
        FILE* resultFile = fopen(resultFilename, "r");
        if (!resultFile) {
            perror("Error opening result file");
            continue;
        }

        double sum = 0;
        fscanf(resultFile, "%lf", &sum);
        totalSum += sum;
        fclose(resultFile);
        remove(resultFilename); // Clean up
    }

    printf("Total sum of squares: %f\n", totalSum);

    return EXIT_SUCCESS;
}
