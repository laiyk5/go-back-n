// void printUDTComm(UDTComm &udt)
// {
//     struct sockaddr_in sname;
//     socklen_t slen = sizeof(sockaddr_in);
//     getsockname(udt.sock, (struct sockaddr *)&sname, &slen);
//     char buffer[16];
//     printf("sockaddr: %s\n sockport: %d\n", 
//             inet_ntop(AF_INET, &sname.sin_addr, buffer, sizeof(sockaddr_in)),
//             ntohs(sname.sin_port));
//     printf("raddr: %s\n rport: %d\n",
//             inet_ntop(AF_INET, &udt.raddr.sin_addr, buffer, sizeof(sockaddr_in)),
//             ntohs(udt.raddr.sin_port));
// }
void printName(struct sockaddr_in &sname){
    char buffer[16];
    printf("\tsockaddr: %s\n\tsockport: %d\n", 
            inet_ntop(AF_INET, &sname.sin_addr, buffer, sizeof(sockaddr_in)),
            ntohs(sname.sin_port));
}

void printSockName(int &sock)
{
    struct sockaddr_in sname;
    socklen_t slen = sizeof(sockaddr_in);
    getsockname(sock, (struct sockaddr *)&sname, &slen);
    printName(sname);
}
