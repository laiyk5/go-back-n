#include "Timer.hpp"

#ifndef STOPNWAIT_HPP
#define STOPNWAIT_HPP

#define MAXSEQ 32767
#define TIMEOUT 0.07


class StopNWaitComm
{
private:
    Socket sock;
    Packet recvpkt;
    Packet sendpkt;
    int seq;    
    Timer timer;
public:
    // for server
    StopNWaitComm(int port):sock(port), seq(0){;}
    // for clinet
    StopNWaitComm(const char * ip, int recvport):seq(0){
        sock.set_remote(ip, recvport);
    }
    
    // use send and recv in pair, their are blocking methods.
    void send(void *data, int datasize){ //发送seq包接收seqack
        sendpkt.set(seq, 0, DATA, data, datasize);
        send_pkt();
    }
    int recv(void *data){ // 接收seq包发送seqack
        sendpkt.set(0, seq, ACK, NULL, 0);
        if(-1 == recv_pkt(DATA)){ // FIN instead of DATA
            return -1;
        }else{
            memcpy(data, recvpkt.data, recvpkt.get_datasize());
            return recvpkt.get_datasize();
        }
    }

    void connect(){
        // connect
        sendpkt.set(seq, 0, SYN, NULL, 0);
        send_pkt();
    }
    void accept(){
        while(true){
            recvpkt.size = sock.recv_from(&recvpkt, MAX_PKTSIZE);
            if((-1 != recvpkt.size )
                && recvpkt.flags & SYN){
                recvpkt.show("<--");
                // syn seq
                seq = recvpkt.seq;
                // update seq;
                seq = (seq + 1) % MAXSEQ;
                // send ack
                sendpkt.set(0, recvpkt.seq, ACK, NULL, 0);
                sock.send(&sendpkt, sendpkt.size);
                sendpkt.show("-->");
                break;
            }
        }
    }
    void disconnect(){
        // 发送FIN包
        sendpkt.set(seq, 0, FIN, NULL, 0);
        send_pkt();
        
        while(-1 != recv_pkt(DATA))
            ;

        // 等待超时
        timer.start();
        while(!timer.timeout(TIMEOUT)){
            recvpkt.size = sock.recv(&recvpkt, MAX_PKTSIZE);
            if(-1 != recvpkt.size && (recvpkt.flags & FIN))
            {
                recvpkt.show("<--WRONG");
                sock.send(&sendpkt, sendpkt.size);
                sendpkt.show("REACKED-->");
                timer.start();
            }
        }
        timer.stop();
    }
    void goodbye(){
        // 发送FIN包
        sendpkt.set(seq, 0, FIN, NULL, 0);
        send_pkt();
    }
    
private:


    void send_pkt(){
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
                seq = (seq + 1) % MAXSEQ;
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
        timer.stop();
    }
    int recv_pkt(int flag){
        while(true){
            recvpkt.size = sock.recv(&recvpkt, MAX_PKTSIZE);
            if(-1 != recvpkt.size){       // 收到
                if( (recvpkt.seq == seq) // 按序 且为 data或fin包
                    && (recvpkt.flags & (flag | FIN)))
                {
                    recvpkt.show("<--Correct");
                    // update seq
                    seq = (seq + 1) % MAXSEQ;
                    // send ack
                    sendpkt.set(0, recvpkt.seq, ACK, NULL, 0);
                    sock.send(&sendpkt, sendpkt.size);
                    sendpkt.show("-->");
                    
                    if(flag == DATA && (recvpkt.flags & FIN)){
                        return -1;
                    }else{
                        return recvpkt.size;
                    }
                }else{
                    recvpkt.show("<--WRONG");
                    sock.send(&sendpkt, sendpkt.size);
                    sendpkt.show("REACKED-->");
                }
            }
        }
    }
};

#endif