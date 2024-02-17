#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int nChildren = atoi(argv[1]);
    int totalNumbers = 10; // Assuming we know the total numbers in advance. Adjust as needed.

    // Adjust the number of children if necessary
    if (nChildren > totalNumbers / 2) {
        nChildren = totalNumbers / 2;
        printf("Number of children adjusted to %d\n", nChildren);
    }

    int numbersPerChild = totalNumbers / nChildren;
    int extraNumbers = totalNumbers % nChildren;

    STARTUPINFO si[nChildren];
    PROCESS_INFORMATION pi[nChildren];
    char cmdLine[BUFFER_SIZE];

    for (int i = 0; i < nChildren; i++) {
        int start = i * numbersPerChild;
        int end = (i == nChildren - 1) ? totalNumbers : start + numbersPerChild + (i == nChildren - 1 ? extraNumbers : 0);

        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        sprintf(cmdLine, "child_process_win.exe %d %d %d", start, end, i);
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i])) {
            fprintf(stderr, "CreateProcess failed (%d).\n", GetLastError());
            exit(EXIT_FAILURE);
        }
    }

    double totalSum = 0;
    for (int i = 0; i < nChildren; i++) {
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);

        char resultFilename[BUFFER_SIZE];
        sprintf(resultFilename, "result_%d.txt", i);
        FILE* resultFile = fopen(resultFilename, "r");
        if (!resultFile) {
            fprintf(stderr, "Error opening result file: %s\n", resultFilename);
            continue;
        }

        double sum = 0;
        fscanf(resultFile, "%lf", &sum);
        totalSum += sum;
        fclose(resultFile);
        remove(resultFilename);
    }

    printf("Total sum of squares: %f\n", totalSum);

    return EXIT_SUCCESS;
}
