#include <iostream>
#include "Utils.h"

const char PORT[] = "3490";
const char HOST_NAME[] = "127.0.0.1";
const int MAX_DATA_SIZE = 100;

using namespace Utils;

int main()
{
    addrinfo* servInfo, * p;
    addrinfo hints = makeHints();

    if (int retVal = getaddrinfo(HOST_NAME, PORT, &hints, &servInfo) == -1)
    {
        std::cout << "getaddrinfo: " << gai_strerror(retVal);
        return 1;
    }

    int sockFd = connectFirstAvail(servInfo, p);

    if (!p)
    {
        std::cout << "Client failed to connect";
        return 1;
    }

    char serverAddrStr[INET6_ADDRSTRLEN];

    inet_ntop(p->ai_family, castInAddrIPv((sockaddr *)p->ai_addr), serverAddrStr, sizeof(serverAddrStr));
    std::cout << "client: connecting to " << serverAddrStr << '\n';

    freeaddrinfo(servInfo);

    char buf[MAX_DATA_SIZE];
    int numBytes;

    if ((numBytes = recv(sockFd, buf, MAX_DATA_SIZE - 1, 0)) == -1)
    {
        perror("recv");
        return 1;
    }

    buf[numBytes] = '\0';

    std::cout << "client - received message: '" << buf << "'\n";

    close(sockFd);

    return 0;
}
