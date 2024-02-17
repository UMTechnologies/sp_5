#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <start_pos> <end_pos> <child_index>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    int childIndex = atoi(argv[3]);
    char resultFilename[260];
    sprintf(resultFilename, "result_%d.txt", childIndex);

    FILE* file = fopen("numbers.txt", "r");
    if (!file) {
        perror("Error opening numbers.txt");
        return EXIT_FAILURE;
    }

    double number, sum = 0;
    int pos = 0;
    while (fscanf(file, "%lf", &number) == 1) {
        if (pos >= start && pos < end) {
            sum += number * number;
        }
        pos++;
    }
    fclose(file);

    file = fopen(resultFilename, "w");
    if (!file) {
        perror("Error creating result file");
        return EXIT_FAILURE;
    }

    fprintf(file, "%f", sum);
    fclose(file);

    return EXIT_SUCCESS;
}
