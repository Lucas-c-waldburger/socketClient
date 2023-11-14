#ifndef SOCKETCLIENT_UTILS_H
#define SOCKETCLIENT_UTILS_H
#include "includes.h"

namespace Utils
{
    void* castInAddrIPv(sockaddr* sa);

    void sigchldHandler(int s);

    addrinfo makeHints();

    int connectFirstAvail(addrinfo* servInfo, addrinfo*& p);

    int setReuse(int sockFd);

    bool reapDeadProcesses();


}


#endif //SOCKETCLIENT_UTILS_H
