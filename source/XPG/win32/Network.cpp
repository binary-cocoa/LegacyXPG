#include "../Network.hpp"

namespace XPG
{
    static WSADATA wsad;
    void NetworkStart()
    {
        WSAStartup(MAKEWORD(2, 2), &wsad);
    }

    void NetworkEnd()
    {
        WSACleanup();
    }
}
