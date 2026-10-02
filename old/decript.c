#include <stdio.h>
#include <stdlib.h>


int main () {
    FILE* inputFile;
    FILE* outputFile;

    inputFile = fopen("input.txt", "r");
    outputFile = fopen("output.txt", "w+");


    if (inputFile == NULL) {
        printf("File is not opened");

        return 1;
    }

    char c;

    while (1) {
        c = fgetc(inputFile) + 1;

        if (!feof(inputFile)) {
            printf("%c", c);
            fprintf(outputFile, "%c", c);
        } else break;
    }

    fclose(inputFile);
}
