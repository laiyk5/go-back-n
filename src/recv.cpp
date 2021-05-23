#define CHUNKSIZE 10
#define SENDPORT 12346
#define RECVPORT 12347
#define TIMEOUT 0.05

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

int main(){
    // init sock
    Socket sock(RECVPORT);

    Packet recvpkt;
    Packet sendpkt;
    
    int seq = 0;

    // accept
    while(true){
        recvpkt.size = sock.recv_from(&recvpkt, MAX_PKTSIZE);
        if((-1 != recvpkt.size )
            && recvpkt.flags & SYN){
            recvpkt.show("<--");
            // syn seq
            seq = recvpkt.seq;
            // update seq;
            seq++;
            // send ack
            sendpkt.set(0, recvpkt.seq, ACK, NULL, 0);
            sock.send(&sendpkt, sendpkt.size);
            sendpkt.show("-->");
            break;
        }
    }

    for(int i = 0; i < 10; i++){
        // recv pkt
        while(true){
            recvpkt.size = sock.recv(&recvpkt, MAX_PKTSIZE);
            if(-1 != recvpkt.size){
                if((recvpkt.flags & DATA)
                    && (recvpkt.seq == seq))
                {
                    recvpkt.show("<--Correct");
                    // update seq
                    seq++;
                    // send ack
                    sendpkt.set(0, recvpkt.seq, ACK, NULL, 0);
                    sock.send(&sendpkt, sendpkt.size);
                    sendpkt.show("-->");
                    break;
                }else{
                    recvpkt.show("<--WRONG");
                    sock.send(&sendpkt, sendpkt.size);
                }
            }
        }
    }

}