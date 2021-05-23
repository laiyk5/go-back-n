// SYSTEM
#include <unistd.h>
#include <sys/time.h>

// C++ Library
#include <iostream>
#include <deque>

// C library
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// network
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>


#define TIMEOUT 0.07
#define SENDPORT (IPPORT_USERRESERVED + 1)
#define RECVPORT (IPPORT_USERRESERVED + 2)

#include "Socket.hpp"
#include "Packet.hpp"
#include "Timer.hpp"
#include "StopNWait.hpp"

#define CHUNKSIZE 20
int main(int argc, char *argv[]){
	printf("Usage: send infilename destip\n");
    
    StopNWaitComm sender(argv[2], RECVPORT);
    FILE * infile = fopen(argv[1], "r");

    sender.connect();
    char buffer[CHUNKSIZE+10];
    int buffersize;
    while(0 == feof(infile)){
        buffersize = fread(buffer, 1, CHUNKSIZE, infile);
        printf("%d %s|\n", buffersize, buffer);
        sender.send(buffer, buffersize);
    }
    sender.disconnect();

    fclose(infile);
}
