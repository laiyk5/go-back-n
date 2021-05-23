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
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(RECVPORT);
	if(-1 == bind(sock, (struct sockaddr*)(&sockaddr), sizeof(struct sockaddr_in)))
	{
		perror("bind(recv)");
		exit(1);
	}else{
        printf("SOCKINFO: \n");
        printSockName(sock);
    }
    
    // accept
    int a = 0;
    struct sockaddr_in recvaddr;
    socklen_t recvaddrsize;
    if(-1 == recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&recvaddr, &recvaddrsize)){
        perror("recvfrom");
    }else{
        printf("PKT Accepted, REMOTE INFO:\n");
        printName(recvaddr);
    }

    // send
    if(-1 == sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&recvaddr, recvaddrsize))
    {
        perror("sendto");
        exit(-1);
    }else{
        printf("PKT SENT\n");
    }

}