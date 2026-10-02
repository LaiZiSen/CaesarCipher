#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFERSIZE 100
#define ENCRIPT 1
#define DECRIPT -1
#define UPPER 126
#define LOWER 32
#define RANGE 96

enum iMode{ 
    EMPTY,
    CLIPBOARD,
    TERMINAL
};

bool getClipboard(char *);
void getString(char *);
void encript(char *, char *, int);
int getMode(char *);
void setClipboard(char *);
void printError(char *);
bool isSpecialChar(char);

int main (int argc, char *argv[]) {
    
    char * input = malloc(sizeof(char)*BUFFERSIZE);
    char * result = malloc(sizeof(char)*BUFFERSIZE);
    int inputMode = EMPTY;
    int mode = 0;
    char command[BUFFERSIZE + 24];

    for(int i = 1; i < argc; i++) {
        if (
                (strcmp(argv[i], "-e") == 0) || 
                (strcmp(argv[i], "e") == 0) ||
                (strcmp(argv[i], "-E") == 0) || 
                (strcmp(argv[i], "E") == 0)) {
            if (mode == 0) {
                mode = ENCRIPT;
            } else {
                printError("TOO MANY ARGUMENTS");
            }
        } else if (
                (strcmp(argv[i], "-d") == 0) || 
                (strcmp(argv[i], "d") == 0) ||
                (strcmp(argv[i], "-D") == 0) || 
                (strcmp(argv[i], "D") == 0)) {
            if (mode == 0) {
                mode = DECRIPT;
            } else {
                printError("TOO MANY ARGUMENTS");
            }
        } else if (
                (strcmp(argv[i], "-c") == 0) || 
                (strcmp(argv[i], "c") == 0) ||
                (strcmp(argv[i], "-C") == 0) || 
                (strcmp(argv[i], "C") == 0)) {
            if (inputMode == EMPTY) {
                inputMode = CLIPBOARD;
            } else {
                printError("TOO MANY ARGUMENTS");
            }
        } else if (
                (strcmp(argv[i], "-t") == 0) || 
                (strcmp(argv[i], "t") == 0) ||
                (strcmp(argv[i], "-T") == 0) || 
                (strcmp(argv[i], "T") == 0)) {
            if (inputMode == EMPTY) {
                inputMode = TERMINAL;
            } else {
                printError("TOO MANY ARGUMENTS");
            }
        } else {
            printError("INVALID ARGUMENT");
        }
    }
    if (inputMode == EMPTY) {
        inputMode = CLIPBOARD;
    }
    if (mode == 0) {
        mode = ENCRIPT;
    }

    if(inputMode == CLIPBOARD) {
        getClipboard(input);
    } else if (inputMode == TERMINAL) {
        getString(input);
    }

    encript(input, result, mode);
    setClipboard(result);

    // switch (argc) {
    //     case 1:
    //         // get input for encript
    //
    //         if (!getClipboard(input)) {
    //             getString(input);
    //         }
    //
    //         encript(input, result, ENCRIPT);
    //
    //         setClipboard(result);
    //
    //         break;
    //
    //     case 2:
    //         // check for type and get input for e / d 
    //         mode = getMode(argv[1]);
    //
    //         if (!getClipboard(input)) {
    //             getString(input);
    //         }
    //
    //         encript(input, result, mode);
    //
    //         setClipboard(result);
    //
    //         break;
    //
    //     case 3:
    //         // check for type and directly encript or decript 
    //         mode = getMode(argv[1]);
    //         input = argv[2];
    //         printf("%-10s: %s\n", "Original", input);
    //         encript(input, result, mode);
    //
    //         setClipboard(result);
    //
    //         break;
    //     default:
    //         printError("INVALID ARGUMENTS");
    // }
}

bool getClipboard(char * input) {
    FILE* clipBoard = popen("termux-clipboard-get", "r");

    if (clipBoard == NULL) {
        return false;
    }

    fread(input, 1, BUFFERSIZE, clipBoard);

    if (strlen(input) == 0) {
        return false;
    } else {return true;}
}

int getMode(char * arg) {
    if ((strcmp(arg, "-e") == 0) || (strcmp(arg, "e") == 0)) {
        return ENCRIPT;
    } 
    else if((strcmp(arg, "-d") == 0) || (strcmp(arg, "d") == 0)) {
        return DECRIPT;
    } else {
        printError("INVALID ARGUMENTS");
    }

    return 0;
}

void encript(char *input, char * output, int mode) {
    int j;

    for(int i = 0; i < strlen(input); i++) {

        if (isSpecialChar(input[i])) {
            j = input[i];
        } else {
            j = input[i] + (i + 1)*mode;
        }
   
        while (j < LOWER) {
            j = j + 95;
        }
        while (j > UPPER) {
            j = j - 95;
        }

        output[i] = j;
    }

    printf("%-10s: %s\n", "Result", output);
}

void getString(char * output) {
    printf("Enter line: ");
    
    fgets(output, BUFFERSIZE, stdin);

    output[strcspn(output, "\n")] = '\0';
}

void setClipboard(char * input) {

    char command[BUFFERSIZE+64];

    sprintf(command, "termux-clipboard-set \'");
    int j = strlen(command);

    for(int i = 0; i < strlen(input); i++) {
        if ( isSpecialChar(input[i]) ) {
            command[j++] = input[i];
        } else if (input[i] != '\'') {
            command[j++] = input[i];
        } else {
            command[j++] = '\'';
            command[j++] = '"';
            command[j++] = input[i];
            command[j++] = '"';
            command[j++] = '\'';
        }
    }
    command[j++] = '\'';
    command[j++] = '\0';

    system(command);
}

void printError(char * msg) {
    printf("%s", msg);
    exit(1);
}

bool isSpecialChar(char c) {
        if (
                c == '\r'||
                c == '\n'||
                c == '\t'||
                c == '\0'
                ) {
            return true;
        } else {return false;}
}
