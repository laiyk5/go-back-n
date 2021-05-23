#ifndef SOCKET_HPP
#define SOCKET_HPP

class Socket
{
private:
    // LOCAL
    int sock;
    struct sockaddr_in sockaddr;
    // REMOTE
    struct sockaddr_in recvaddr;
    socklen_t recvaddrsize;
public:
    // client
    Socket();
    // server
    Socket(int recvport);
    void set_remote(const char * ip, int port);
    void send(void *pkt, int size);
    int recv_from(void *pkt, int maxsize);
    int recv(void *pkt, int maxsize);
    void printName(struct sockaddr_in &sname);
    void printSockName(int &sock);

};

Socket::Socket(){
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCKINFO: \n");
    printSockName(sock);
}

Socket::Socket(int recvport):Socket(){
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(RECVPORT);
    if(-1 == bind(sock, (struct sockaddr*)(&sockaddr), sizeof(struct sockaddr_in)))
    {
        perror("bind(recv)");
        exit(1);
    }else{
        printf("ADDR BINDED. SOCKINFO: \n");
        printSockName(sock);
    }
}
void Socket::set_remote(const char * ip, int port){
    recvaddrsize = sizeof(struct sockaddr_in);
    recvaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(recvaddr.sin_addr.s_addr));
    recvaddr.sin_port = htons(port);
    printf("Remote SETTED: \n");
    printName(recvaddr);
}
void Socket::send(void *pkt, int size)
{
    if(-1 == sendto(sock, pkt, size, 0, (struct sockaddr *)&recvaddr, recvaddrsize))
    {
        perror("sendto");
        exit(-1);
    }else{
        printf("PKT SENT\n");
    }
}

int Socket::recv_from(void *pkt, int maxsize){
    int size = 0;
    if(-1 == (size = recvfrom(sock, pkt, maxsize, 0, (struct sockaddr *)&recvaddr, &recvaddrsize))){
        perror("recvfrom");
        exit(-1);
    }else{
        printf("PKT RECV, REMOTE INFO:\n");
        printName(recvaddr);
    }
    return size;
}

int Socket::recv(void *pkt, int maxsize){
    int size = 0;
    if(-1 == (size = ::recv(sock, pkt, maxsize, 0))){
        perror("recv");
        exit(-1);
    }else{
        printf("PKT ACKED\n");
    }
    return size;
}

void Socket::printName(struct sockaddr_in &sname){
    char buffer[16];
    printf("\tsockaddr: %s\n\tsockport: %d\n", 
            inet_ntop(AF_INET, &sname.sin_addr, buffer, sizeof(sockaddr_in)),
            ntohs(sname.sin_port));
}

void Socket::printSockName(int &sock)
{
    struct sockaddr_in sname;
    socklen_t slen = sizeof(sockaddr_in);
    ::getsockname(sock, (struct sockaddr *)&sname, &slen);
    printName(sname);
}


#endif