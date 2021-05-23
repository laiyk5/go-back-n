#ifndef PACKET_HPP
#define PACKET_HPP


#define HEADLEN 16
#define MAX_DATASIZE 64 // 16 + 64 = 80 < 128 < 1024/8 < 1500/8
#define MAX_PKTSIZE (HEADLEN+MAX_DATASIZE)

enum FLAG{SYN=1, ACK=2, FIN=4, DATA=8};

class Packet
{
public:
    int seq;
    int ack;
    int flags; // SYN, ACK, FIN, DATA
    int size;
    char data[MAX_DATASIZE];

public:
    void set(int _seq, int _ack, int _flags, void *_data, int _datasize)
    {
        seq = _seq;
        ack = _ack;
        flags = _flags;
        size = HEADLEN + _datasize;
        if(data != NULL){
            memcpy(data, _data, _datasize);
        }
    }
    void show(const char prefix[])
    {
        const char *flag;
        switch(this->flags){
            case SYN: flag = "SYN"; break;
            case ACK: flag = "ACK"; break;
            case FIN: flag = "FIN"; break;
            case DATA: flag = "DATA"; break;
            default: perror("show: flags 1"); exit(-1); break;
        }
        switch(this->flags){
            case SYN: case DATA: case FIN:
                printf("%s [%s] seq == %d\n", prefix, flag, seq);
                break;
            case ACK: 
                printf("%s [%s] ack == %d\n", prefix, flag, ack);
                break;
            default: perror("show: flags 2"); exit(-1); break;
        }
    }

    int get_datasize(){
        return size - HEADLEN;
    }
};


#endif