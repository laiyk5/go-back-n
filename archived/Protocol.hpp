#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "gbn.hpp"
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

// #include <cstdlib>
// #include <cstring>
// #include <cerrno>
// #include <cstdio>



class Packet
{
public:
    uint32_t seq;
    uint32_t ack;
    uint32_t flags;
    int32_t size;
    char data[DATASIZE_MAX];
public:
    Packet(){;}
    
    Packet(int s, int a, int f, void *d, int datasize)
        : seq(s), ack(a), flags(f), size(HEADSIZE + datasize)
    {
        if(d != NULL){
            memcpy(data, d, size);
        }
    }
    void set_pkt(int s, int a, int f, void *d, int datasize)
    {
        seq = s;
        ack = a;
        flags = f;
        size = HEADSIZE + datasize;
        if(d != NULL){
            memcpy(data, d, size);
        }
    }
    uint32_t get_seq()
    {
        return this->seq;
    }
};


// UDPsocket，可以设置自己的名字，可以设置目的地，可以收发包，可以建立连接（根据收到包的地址修改目的地）
class UDTComm
{
public:
    int sock;
    void * rbuff;
    int rsize;
    struct sockaddr_in raddr;
    struct sockaddr_in sname;
    socklen_t raddrsize;
public:
    UDTComm(const char *ip, const int port){
        sock = socket(PF_INET, SOCK_DGRAM, 0);
        // set sockname
        set_sock(ip, port);
        
        rbuff = malloc(PKTSIZE_MAX);
    }
    ~UDTComm(){
        shutdown(sock, 2);
        free(rbuff);
    }

// addr
    void set_recv(const char* ip, int port)
    {
        raddr.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &(raddr.sin_addr.s_addr));
        raddr.sin_port = htons(port);
    }
    void set_sock(const char *ip, int port)
    {
        sname.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &sname.sin_addr);
        sname.sin_port = htons(port);
        if(-1 == bind(sock, (struct sockaddr *)(&sname), sizeof(struct sockaddr_in))){
            perror("bind");
            exit(-1);
        }
    }
// functions
    void send(void * pkt, int size){
        sendto(sock, pkt, size, 0, (struct sockaddr *)&raddr, raddrsize);
    }

// 阻塞式 accept
    void accept(void *pkt, int *size)
    {
        raddrsize = sizeof(struct sockaddr_in);
        *size = recvfrom(sock, pkt, PKTSIZE_MAX, 0, (struct sockaddr *)&raddr, &raddrsize);
    }
// 非阻塞 receive
    bool receive(void * pkt, int *size)
    {
        *size = recv(sock, pkt, PKTSIZE_MAX, MSG_DONTWAIT);
        return *size <= 0;
    }
};

#endif