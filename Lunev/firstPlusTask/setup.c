#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void prepareForwardFifo() {
    mkfifo("forwardFifo", 0644);
}
void prepareBackwardFifo() {
    mkfifo("backwardFifo", 0644);
}

int main(int argc, char** argv) {
    prepareForwardFifo();
    prepareBackwardFifo();
    return 0;
}

