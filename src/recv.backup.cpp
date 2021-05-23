
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


#include "Socket.hpp"
#include "Packet.hpp"
#include "StopNWait.hpp"

#define SENDPORT (IPPORT_USERRESERVED + 1)
#define RECVPORT (IPPORT_USERRESERVED + 2)
#define CHUNKSIZE 20

int main(int argc, char *argv[]){
    printf("Usage: recv outfilename");
    StopNWaitReceiver receiver(RECVPORT);

    FILE * outfile = fopen(argv[1], "w");
    char buffer[CHUNKSIZE + 10];
    int buffersize;

    receiver.accept();
    while(true){
        buffersize = receiver.recv(buffer);
        printf("%d\n",buffersize);
        fwrite(buffer, 1, buffersize, outfile);
        fflush(outfile);
    }
    return 0;
}
