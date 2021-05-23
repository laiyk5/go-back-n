#ifndef RDTSENDER_HPP
#define RDTSENDER_HPP

#define WNDSIZE_MAX 5
#define SEQSPACE (WNDSIZE_MAX*4)

#include "gbn.hpp"
// #include <deque>
// #include "Protocol.hpp"
// #include "Timer.hpp"

class RDTSender
{
private:
    // buffers
    std::deque<Packet> wnd;
    Packet sendpkt;
    Packet recvpkt;
    uint32_t baseseq, nextseq;
    // facilities
    UDTComm udt;    // basic sender
    Timer timer;    // real timer
public:
    RDTSender(const char *ip, const int port)
    : baseseq(0), nextseq(0), udt(ip, port){
        ;
    }
    void connect(const char *ip)
    {
        udt.set_recv(ip, RECVPORT);
        sendpkt.set_pkt(nextseq, 0, SYN, NULL, 0);
        send_pkt(sendpkt);
    }
    void send(void *data, int datasize){
        sendpkt.set_pkt(nextseq, 0, 0, data, datasize);
        send_pkt(sendpkt);
    }
    void disconnect(){
        //发送FIN包
        sendpkt.set_pkt(nextseq, 0, FIN, NULL, 0);
        send_pkt(sendpkt);
        // 接收所有ack，还要防止FIN包超时
        while(!isempty()){
            // 确认机制
            while(rnp_ack()){
                ;
            }
            if(!isempty()){
                timer.start();
            }else{
                timer.stop();
            }
            // 超时重传
            if(timer.istimeout()){
                resendAll();
                timer.start();
            }
        }
        // 接收并ack对方的FIN包，等待超时
        timer.stop();
        while(!timer.istimeout()){
            if(recv_fin()){ // 每每收到fin包都ACK并重新等待
                // 发送ACK包
                sendpkt.set_pkt(0, nextseq, ACK, NULL, 0);
                udt.send(&sendpkt, sendpkt.size);
                timer.start();
            }
        }
    }
    
private:
    //功能：将包发送出去，并确保有ack收到
    void send_pkt(Packet &pkt)
    {
        // 每次send前的例行检查
        // ack 在 timeout之前做，以免send间隔太久没机会ack导致timeout
        while(rnp_ack()){     // 每次send顺便清空ack缓存
            ;
        }
        if(!isempty()){
            timer.start(); // 成功ack，重启计时器
        }
        if(timer.istimeout()){
            resendAll();
            timer.start();
        }

        // 满了一直尝试接收ack和重传直到不满
        while(isfull()){
            // 一直尝试接收ack直到超时
            bool flag;
            while(!timer.istimeout() 
                && !(flag = rnp_ack()))
            {
                ;
            }
            if(flag == true){   // 成功ACK，重启计时器
                timer.start();
                // break; // 多余，此时必notfull
            }
            resendAll();        // 超时，重启计时器
            timer.start();
        }
        if(isempty()){
            timer.start();
        }
        udt.send(&pkt, pkt.size);
        wnd.push_back(pkt);
        nextseq = (nextseq + 1) % SEQSPACE; //update nextseq
    }

    bool recv_fin()
    {
        if(udt.receive(&recvpkt, &(recvpkt.size))
            &&(recvpkt.flags & FIN))
            return true;
        else
            return false;
    }

    bool rnp_ack() // recv and proccess ack, buggy, make it unblocked.
    {
        if(udt.receive(&recvpkt, &(recvpkt.size))  // Recv Success
            && (recvpkt.flags & ACK)             // ACK Pkt
            && (recvpkt.ack >= baseseq && recvpkt.ack < nextseq)) // Right ack value
        {
            for(uint32_t i = baseseq; i <= recvpkt.ack; i++) // pop [baseseq, recvpkt.ack]
            {
                wnd.pop_front();
            }
            baseseq = (recvpkt.ack + 1) % SEQSPACE; // update baseseq
            return true;
        }
        return false;
    }

    void resendAll()
    {
        for(auto iter = wnd.begin(); iter != wnd.end(); iter++)
        {
            udt.send(&(*iter), (*iter).size);
        }
    }
    bool isfull(){
        return ((baseseq + WNDSIZE_MAX) % SEQSPACE) == nextseq;
    }
    bool isempty(){
        return baseseq == nextseq;
    }
};

#endif