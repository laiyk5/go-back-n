#include "gbn.hpp"
#include "FileSender.hpp"
#include <cstdlib>
#include <iostream>

void printSockName(UDTComm &udt)
{
    struct sockaddr_in sname;
    socklen_t slen = sizeof(sockaddr_in);
    getsockname(udt.sock, (struct sockaddr *)&sname, &slen);
    char buffer[16];
    printf("sockaddr: %s\n sockport: %d\n", 
            inet_ntop(AF_INET, &sname.sin_addr, buffer, sizeof(sockaddr_in)),
            ntohs(sname.sin_port));
    printf("raddr: %s\n rport: %d\n",
            inet_ntop(AF_INET, &udt.raddr.sin_addr, buffer, sizeof(sockaddr_in)),
            ntohs(udt.raddr.sin_port));
}

void TestUDTComm(){
    UDTComm udt("127.0.0.1", SENDPORT);
    udt.set_recv("127.0.0.1", RECVPORT);
    printSockName(udt);
    int a = 127;
    udt.send(&a, sizeof(a));
}

int main(int argc, char * argv[]){
    if(argc != 3){
        std::cerr << "args: filename ip" << std::endl;
    }
    TestUDTComm();
    // FileSender FS;
    // // send file "filename" to host "ip"
    // FS.send_file(argv[1], argv[2]);
}