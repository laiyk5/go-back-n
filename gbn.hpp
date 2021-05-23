#ifndef GBN_HPP
#define GBN_HPP

#define CHUNKSIZE 10
#define SENDPORT 12346
#define RECVPORT 12347
#define TIMEOUT 0.05

#define HEADSIZE 16
#define DATASIZE_MAX 32 // 16 + 64 = 80 < 128 < 1024/8 < 1500/8
#define PKTSIZE_MAX (HEADSIZE+DATASIZE_MAX)

#define SYN 1
#define ACK 2
#define FIN 4

// SYSTEM
#include <unistd.h>
#include <sys/time.h>

// C++ Library
#include <iostream>
#include <deque>

// C library
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// network
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

// my files
#include "Protocol.hpp"
#include "Timer.hpp"

#include "RDTSender.hpp"
#include "FileSender.hpp"

#include "RDTReceiver.hpp"
#include "FileReceiver.hpp"

#endif