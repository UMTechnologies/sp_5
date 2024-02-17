#include <stdio.h>
#include <stdlib.h>
#include "windows.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <start_pos> <end_pos> <child_index>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    int childIndex = atoi(argv[3]);
    char resultFilename[260];
    sprintf(resultFilename, "result_%d.txt", childIndex);

    FILE* file = fopen("numbers.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening numbers.txt\n");
        exit(EXIT_FAILURE);
    }

    double sum = 0.0, number;
    int pos = 0;
    while (fscanf(file, "%lf", &number) == 1) {
        if (pos >= start && pos < end) {
            sum += number * number;
        }
        pos++;
    }
    fclose(file);

    FILE* resultFile = fopen(resultFilename, "w");
    if (!resultFile) {
        fprintf(stderr, "Error creating result file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(resultFile, "%f", sum);
    fclose(resultFile);

    Sleep(50000);

    return 0;
}
