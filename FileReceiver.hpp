#ifndef FILERECEIVER_HPP
#define FILERECEIVER_HPP

#include "gbn.hpp"
// #include "RDTReceiver.hpp"

class FileReceiver
{
private:
    void * dbuff; // data buffer
    RDTReceiver rdtr; // rdt receiver
public:
    FileReceiver():rdtr("127.0.0.0", RECVPORT)
    {
        dbuff = malloc(CHUNKSIZE);
    }
    ~FileReceiver()
    {
        free(dbuff);
    }

    void receive_file(const char * filename)
    {
        FILE * outfile = fopen(filename, "w");
        rdtr.accept();
        int size;
        while(!rdtr.fin()){
            rdtr.receive(&dbuff, &size);
            fwrite(dbuff, 1, size, outfile);
        }
        rdtr.disconnect();
        fclose(outfile);
    }

};


#endif