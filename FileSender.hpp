#ifndef FILESENDER_HPP
#define FILESENDER_HPP

#include "gbn.hpp"
// #include "RDTSender.hpp"
// #include <cstring>
// #include <cstdlib>

class FileSender
{
private:
    void *cbuff; // chunk buffer
    RDTSender rdts;
public:
    FileSender():rdts("127.0.0.1", SENDPORT)
    {
        cbuff = malloc(sizeof(DATASIZE_MAX));
    }
    ~FileSender()
    {
        free(cbuff);
    }

    void send_file(const char* filename, const char *ip)
    {   
        int size;
        FILE * infile = fopen(filename, "r");

        rdts.connect(ip);

        size = fread(cbuff, 1, CHUNKSIZE, infile);
        while(0 == feof(infile))
        {
            rdts.send(cbuff, size);
            size = fread(cbuff, 1, CHUNKSIZE, infile);
        }

        rdts.disconnect();

        fclose(infile);
    }
};

#endif