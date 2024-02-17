#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path_to_numbers_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    double number, sum = 0.0;
    while (fscanf(file, "%lf", &number) == 1) {
        sum += number * number;
    }
    fclose(file);

    printf("Partial sum of squares: %f\n", sum);
    return 0;
}
