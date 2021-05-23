#define CHUNKSIZE 10
#define SENDPORT 12346
#define RECVPORT 12347
#define TIMEOUT 0.05

#define HEADSIZE 16
#define DATASIZE_MAX 32 // 16 + 64 = 80 < 128 < 1024/8 < 1500/8
#define PKTSIZE_MAX (HEADSIZE+DATASIZE_MAX)

#define SYN 1
#define ACK 2
#define FIN 4

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



#include "debug.hpp"

int main(){
    // init sock
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCKINFO: \n");
    printSockName(sock);

    // REMOTE
    struct sockaddr_in recvaddr;
    socklen_t recvaddrsize = sizeof(struct sockaddr_in);
    recvaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(recvaddr.sin_addr.s_addr));
    recvaddr.sin_port = htons(RECVPORT);
    printf("Remote INFO: \n");
    printName(recvaddr);


    // connect
    int a = 127;
    if(-1 == sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&recvaddr, recvaddrsize))
    {
        perror("sendto");
        exit(-1);
    }else{
        printf("PKT SENT\n");
    }

    // recv
    if(-1 == recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&recvaddr, &recvaddrsize)){
        perror("recvfrom");
        exit(-1);
    }else{
        printf("PKT ACKED, REMOTE INFO:\n");
        printName(recvaddr);
    }
}