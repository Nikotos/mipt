#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//------------------CONFIG------------------------
#define BUFFER_SIZE 1024
#define FIFO_PATH "../fifo"
char globalStaticBuffer[BUFFER_SIZE];
//------------------------------------------------



int continuoslyWriteFromFileToPipe(int inputFileDescriptor, int pipeFileDescriptor) {
    if (pipeFileDescriptor == -1) {
        printf("Pipe wasn't open!\n");
        return 1;
    }
    if (inputFileDescriptor == -1) {
        printf("Problems with input file!");
        return 1;
    }
    int writtenDataAmount = 0;
    while ((writtenDataAmount = read(inputFileDescriptor, globalStaticBuffer, BUFFER_SIZE)) &&  writtenDataAmount > 0) {
        write(pipeFileDescriptor, globalStaticBuffer, writtenDataAmount);
    }
    return 0;
}



int main(int argc, char** argv) {
    if (argc == 2) {
        printf("Opening write side of fifo-pipe\n");
        //-------------SOME SETUP-----------------------------------------
        int connectionSettingPipe = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
        if (connectionSettingPipe == -1) {
            printf("Cant open fifo\n");
            return 1;
        }
        printf("Connection via pipe established\n");
        int inputFileDescriptor = open(argv[1], O_RDONLY);
        if (inputFileDescriptor == -1) {
            printf("Cant create output file\n");
            return 1;
        }
        int pid = 0;
        int dataFifo = 0;
        char dataFifoFullName[50] = {0};
        //----------------------------------------------------------------
        pid = getpid();
        printf("I am process - [%d], writing my pid to fifo\n", pid);
        int ans = write(connectionSettingPipe, &pid, sizeof(int));
        sprintf(dataFifoFullName, "../dataFifo%d", pid);
        mkfifo(dataFifoFullName, 0644);
        printf("I am process - [%d], dataFifo created!\n", pid);
        
        dataFifo = open(dataFifoFullName, O_WRONLY);
        if (dataFifo == -1) {
            printf("Error while creating second fifo!\n");
            return 1;
        }
        
        printf("I am process - [%d], started writing to data pipe\n", pid);
        continuoslyWriteFromFileToPipe(inputFileDescriptor, dataFifo);
        printf("I am process - [%d], finished writing to data pipe\n", pid);
        
    } else {
        printf("Inappropriate amount of arguments!");
        return 1;
    }
    return 0;
}

