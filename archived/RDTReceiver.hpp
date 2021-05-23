#ifndef RDTRECEIVER_HPP
#define RDTRECEIVER_HPP

#include "gbn.hpp"
// #include "Protocol.hpp"
// #include "Timer.hpp"

class RDTReceiver
{
private:
    Packet rpkt;
    Packet spkt;
    int rsize;
    int ssize;
    uint32_t seq;
    bool fin_flag;
    //facility
    UDTComm udt;
    Timer timer;
public:
    RDTReceiver(const char *ip, const int port)
    :seq(0), fin_flag(false), udt(ip, port)
    {
        ;
    }
    void accept(){
        // set udt communicator
        do{
            udt.accept(&rpkt, &rsize);
        }while((rpkt.flags & SYN) == 0);
        // set rdt
        seq = rpkt.seq;
    }
    // after fin is detected
    void receive(void * data, int *size){
        // 阻塞接收
        while(!udt.receive(&rpkt, size)){
            ;
        }
        // 过滤失序包
        if(rpkt.seq != seq){
            sendack();
            return;
        }
        // 接收正确包
        seq = (seq + 1) % SEQSPACE;
        sendack();
        *size -= HEADSIZE;
        memcpy(data, &rpkt.data, *size);
        // 检测正确序的FIN包
        if(rpkt.flags & FIN){
            fin_flag = true;
        }
    }
    bool fin(){
        return fin_flag;
    }
    void disconnect(){
        // 发一个FIN包
        // 等待sender唯一的ACK，并确认失序，超时重传。
        sendfin();

        timer.start();
        while(true){
            // 例行检查：超时重传fin
            if(timer.istimeout()){
                sendfin();
                timer.start();
            }
            // 主要部分
            int recv_flag = false;
            while(!timer.istimeout() && !(recv_flag = udt.receive(&rpkt, &rsize))){
                ;
            }
            // 累积确认失序包 or 包序正确，结束
            if(recv_flag && rpkt.flags == ACK){
                break;
            }else{
                sendack();
            }
        }
    }
private:
    void sendack(){
        spkt.set_pkt(0, seq, ACK, NULL, 0);
        udt.send(&spkt, HEADSIZE);
    }
    void sendfin(){
        spkt.set_pkt(0, 0, FIN, NULL, 0);
        udt.send(&spkt, HEADSIZE);
    }
};

#endif