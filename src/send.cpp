#define CHUNKSIZE 10
#define SENDPORT 12346
#define RECVPORT 12347

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
#include "Timer.hpp"

#define TIMEOUT 0.07

int main(int argc, char *argv[]){
	printf("Usage: send hostname\n");
    // init sock
    Socket sock;

    int data = 0;
    int datasize = sizeof(int);

    Timer timer;
    Packet sendpkt;
    Packet recvpkt;

    int state = 0;
    int seq = 0;

    // REMOTE
    sock.set_remote(argv[1], RECVPORT);

    // connect
    sendpkt.set(seq, 0, SYN, &data, datasize);
    sock.send(&sendpkt, sendpkt.size);
    sendpkt.show("-->");
    timer.start();

    // recv ack -- stop-and-wait for ack
    while(true){
        recvpkt.size = sock.recv(&recvpkt, MAX_PKTSIZE);
        if((-1 != recvpkt.size)
            && (recvpkt.flags & ACK) 
            && (recvpkt.ack == seq)){
            recvpkt.show("<--");
            seq++;
            timer.stop();
            break;
        }
        if(timer.timeout(TIMEOUT)){
            // resend
            sock.send(&sendpkt, sendpkt.size);
            sendpkt.show("RE-->");
            timer.start();
        }
    }

    for(int i = 0; i < 10; i++){
        // send pkt with seq 1 2 3 ... 
        sendpkt.set(seq, 0, DATA, &data, datasize);
        sock.send(&sendpkt, sendpkt.size);
        sendpkt.show("-->");
        timer.start();
        // recv ack -- stop-and-wait for ack
        while(true){
            recvpkt.size = sock.recv(&recvpkt, MAX_PKTSIZE);
            if((-1 != recvpkt.size)
                && (recvpkt.flags & ACK)
                && (recvpkt.ack == seq)){
                recvpkt.show("<--");
                seq++;
                timer.stop();
                break;
            }
            if(timer.timeout(TIMEOUT)){
                // resend
                sock.send(&sendpkt, sendpkt.size);
                sendpkt.show("RE-->");
                timer.start();
            }
        }
    }
}