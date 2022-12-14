
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

#define SENDPORT (IPPORT_USERRESERVED + 1)
#define RECVPORT (IPPORT_USERRESERVED + 2)
#define CHUNKSIZE 20

#include "Socket.hpp"
#include "Packet.hpp"
#include "StopNWait.hpp"

int main(int argc, char *argv[]){
    printf("Usage: recv outfilename");
    StopNWaitComm receiver(RECVPORT);

    FILE * outfile = fopen(argv[1], "w");
    char buffer[CHUNKSIZE + 10];
    int buffersize;

    receiver.accept();
    while(-1 != (buffersize = receiver.recv(buffer))){
        printf("%d %s|\n",buffersize, buffer);
        fwrite(buffer, 1, buffersize, outfile);
        fflush(outfile);
    }
    receiver.goodbye();
    return 0;
}
