
#include "Utils.h"

void* Utils::castInAddrIPv(sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
        return &(((sockaddr_in*)sa)->sin_addr);

    return &(((sockaddr_in6*)sa)->sin6_addr);
}

void Utils::sigchldHandler(int s)
{
    int savedErrno = errno;

    while(waitpid(-1, nullptr, WNOHANG) > 0);

    errno = savedErrno;
}

addrinfo Utils::makeHints()
{
    addrinfo hints{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    return hints;
}

int Utils::connectFirstAvail(addrinfo* servInfo, addrinfo*& p)
{
    int sockFd;

    for (p = servInfo; p != nullptr; p = p->ai_next)
    {
        sockFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockFd == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockFd, p->ai_addr,  p->ai_addrlen) == -1)
        {
            close(sockFd);
            perror("client: connect");
            continue;
        }

        break;
    }

    return sockFd;
}

int Utils::setReuse(int sockFd)
{
    int reuse = 1;
    int successFlags = (SO_REUSEADDR | SO_REUSEPORT);

    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        successFlags ^= SO_REUSEADDR;
    }

    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt(SO_REUSEPORT) failed");
        successFlags ^= SO_REUSEPORT;
    }

    return successFlags;
}

bool Utils::reapDeadProcesses()
{
    struct sigaction sigAct;

    sigAct.sa_handler = sigchldHandler;
    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sigAct, nullptr) == -1)
    {
        perror("sigaction");
        return false;
    }

    return true;
}
