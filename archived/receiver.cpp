#include "gbn.hpp"

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
    UDTComm udt("0.0.0.0", RECVPORT);
    printSockName(udt);
    int a;
    int size;
    size = recv(udt.sock, &a, sizeof(int), 0);
    printf("a = %d, size = %d\n", a, size);
}
int main(int argc, char * argv[]){
    // if(argc != 2){
    //     std::cerr << "args: filename" << std::endl;
    // }
    TestUDTComm();
    // FileReceiver FR;
    // // receive file and store it in file "filename"
    // FR.receive_file(argv[1]);
}