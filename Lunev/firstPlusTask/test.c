#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

long int getPositiveNumberFromInput(char** argv) {
    char* endptr = NULL;
    long int number = strtol(argv[0], &endptr, 0);
    if (number == 0) {
        printf("Inappropriate Symbol!\n");
        return -1;
    }
    else if (*endptr != 0) {
        printf("Inappropriate Symbol in the end - [%c]\n", *endptr);
        return -1;
    }
    else if (errno == ERANGE) {
        printf("Number out of range!\n");
        return -1;
    }
    else if (number < 0) {
        printf("Number is abive ZERO!!!");
        return -1;
    }
    else {
        return number;
    }
}

int main(int argc, char** argv) {
    if (argc == 2) {
        printf("%d\n", kill(getPositiveNumberFromInput(argv + 1), 0));
    } else {
        printf("Inappropriate amount of arguments!");
        return 1;
    }
    return 0;
}



