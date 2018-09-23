#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//------------------CONFIG----------------------
#define PACKET_SIZE 1024
#define HEADER_SIZE 24
#define OUTPUT_FILENAME "output"
#define FORWARD_FIFO_PATH "./forwardFifo"
#define BACKWARD_FIFO_PATH "./backwardFifo"
//----------------------------------------------


//------------------SOME TRICKS----------------------
#define SAFETY_LAUNCH(FUNCTION_CALL_RETURN_VALUE)                \
    if (FUNCTION_CALL_RETURN_VALUE) {                            \
        return 1;                                                \
}
//---------------------------------------------------



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
    while ((writtenDataAmount = read(inputFileDescriptor, &globalStaticBuffer, PACKET_SIZE)) &&  writtenDataAmount > 0) {
        write(pipeFileDescriptor, globalStaticBuffer, writtenDataAmount);
    }
    return 0;
}

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
    while ((readDataAmount = read(pipeFileDescriptor, &globalStaticBuffer, PACKET_SIZE)) && readDataAmount > 0) {
        write(outputFileDescriptor, &globalStaticBuffer, readDataAmount);
    }
    chmod(OUTPUT_FILENAME, 0000666);
    return 0;
}


uint32_t hash(char* buffer, int bufferSize) {
    uint32_t hash = 0;
    for(int i = 0; i < bufferSize; i++)
    {
        hash = hash * 31 + buffer[i];
    }
    return hash;
}


void makeHeader(char* packet, uint32_t packetNumber, uint32_t senderPID, uint32_t hashValue) {
    int* packetAdaptation = (uint32_t*) packet;
    packetAdaptation[0] = packetNumber;
    packetAdaptation[1] = senderPID;
    packetAdaptation[2] = hashValue;
}

void waitForAnswer() {
    
}



int main(int argc, char** argv) {
    if (argc == 2) {
        //-------------SOME SETUP-----------------------------------------
        char* inputFilename = argv[1];
        int forwardPipe = open(FORWARD_FIFO_PATH, O_WRONLY);
        int backwardPipe = oper(BACKWARD_FIFO_PATH, O_RDONLY);
        int inputFileDescriptor;
        char packet[BUFFER_SIZE];  //I mean bytes here
        //----------------------------------------------------------------
        
        if ((inputFileDescriptor = open(inputFilename, O_RDONLY) && inputFileDescriptor == -1) {
            printf("Can't open input file!\n");
            return 1;
        }
            
        int readDataAmount = 0;
        int writtenDataAmount = 0;
        uint32_t packetNumber = 0;
        uint32_t senderPID = getpid();
        while(true) {
            readDataAmount = read(inputFileDescriptor, packet + HEADER_SIZE, PACKET_SIZE - HEADER_SIZE);
            
            if (readDataAmount == 0) {
                break;
            }
            
            makeHeader(packet, pocketNumber, senderPID, hash(packet + HEADER_SIZE, PACKET_SIZE - HEADER_SIZE));
            write(forwardPipe, packet, PACKET_SIZE);
            waitForAnswer();
            packetNumber++;
        }
            
        
        
        
    } else {
        printf("Inappropriate amount of arguments!");
        return 1;
    }
    return 0;
}

