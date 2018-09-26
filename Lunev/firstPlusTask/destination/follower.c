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


int continuoslyReadFromPipeToFile(int pipeFileDescriptor, int outputFileDescriptor) {
    if (pipeFileDescriptor == -1) {
        printf("Pipe wasn't open!\n");
        return 1;
    }
    if (outputFileDescriptor == -1) {
        printf("Problems with output file!");
        return 1;
    }
    int readDataAmount = 0;
    while ((readDataAmount = read(pipeFileDescriptor, &globalStaticBuffer, BUFFER_SIZE)) && readDataAmount > 0) {
        write(outputFileDescriptor, &globalStaticBuffer, readDataAmount);
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 2) {
        //-------------SOME SETUP----------------------------------------------
        int connectionSettingPipe = open(FIFO_PATH, O_RDONLY);
        if (connectionSettingPipe == -1) {
            printf("Cant open fifo\n");
            return 1;
        }
        int outputFileDescriptor = open(argv[1], O_CREAT | O_WRONLY);
        if (outputFileDescriptor == -1) {
            printf("Cant create output file\n");
            return 1;
        }
        int dataFifo = 0;
        int pid = 0;
        char dataFifoFullName[50];
        //----------------------------------------------------------------------
        printf("trying to get streamers PID from fifo\n");
        read(connectionSettingPipe, &pid, sizeof(int));
        printf("recieving file from procces - [%d]\n", pid);
        sprintf(dataFifoFullName, "../dataFifo%d", pid);
        
        dataFifo = open(dataFifoFullName, O_RDONLY);
        if (dataFifo == -1) {
            printf("Cant open dataFifo!\n");
            return 1;
        }
        
        unlink(dataFifoFullName);
                        
        continuoslyReadFromPipeToFile(dataFifo, outputFileDescriptor);
        chmod(argv[1], 0000666);

    } else {
        printf("Inappropriate amount of arguments!");
        return 1;
    }
    return 0;
}


