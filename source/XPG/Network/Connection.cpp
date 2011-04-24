#include <XPG/Network/Connection.hpp>
#include <sstream>
#include <iostream>
using namespace std;

namespace XPG
{
    static void* get_in_addr(sockaddr* inSockAddr)
    {
        if (inSockAddr->sa_family == AF_INET)
            return &(((sockaddr_in*)inSockAddr)->sin_addr);

        return &(((sockaddr_in6*)inSockAddr)->sin6_addr);
    }

    Connection::Connection() : mSocket(0), mServInfo(NULL)
    {
    }

    Connection::~Connection()
    {
        if (mSocket)
        {
            closesocket(mSocket);
        }

        if (mServInfo)
        {
            freeaddrinfo(mServInfo);
            mServInfo = NULL;
        }
    }

    void Connection::listen(int16u inPort)
    {
        stringstream ss;
        ss << inPort;
        this->listen(ss.str().c_str());
    }

    void Connection::listen(const char* inPort)
    {
        if (!mSocket)
        {
            memset(&mHints, 0, sizeof mHints);
            mHints.ai_family = AF_UNSPEC;
            mHints.ai_socktype = SOCK_DGRAM;
            mHints.ai_flags = AI_PASSIVE;

            if (getaddrinfo(NULL, inPort, &mHints, &mServInfo))
            {
                cerr << "Connection::listen -- getaddrinfo\n";
                return;
            }

            for (mP = mServInfo; mP; mP = mP->ai_next)
            {
                mSocket = socket(mP->ai_family, mP->ai_socktype,
                    mP->ai_protocol);
                if (mSocket == -1)
                {
                    cerr << "Connection::connect -- socket\n";
                    continue;
                }

                if (bind(mSocket, mP->ai_addr, mP->ai_addrlen) == -1)
                {
                    closesocket(mSocket);
                    mSocket = 0;
                    cerr << "Connection::connect -- bind\n";
                    continue;
                }

                break;
            }

            if (!mP)
            {
                mSocket = 0;
                cerr << "Connection::listen -- failed to bind socket\n";
                return;
            }
        }
    }

    void Connection::connect(const char* inAddress, int16u inPort)
    {
        stringstream ss;
        ss << inPort;
        this->connect(inAddress, ss.str().c_str());
    }

    void Connection::connect(const char* inAddress, const char* inPort)
    {
        memset(&mHints, 0, sizeof mHints);
        mHints.ai_family = AF_UNSPEC;
        mHints.ai_socktype = SOCK_DGRAM;

        if (getaddrinfo(inAddress, inPort, &mHints, &mServInfo))
        {
            cerr << "Connection::connect -- getaddrinfo\n";
            return;
        }

        if (!mSocket)
        {
            for (mP = mServInfo; mP; mP = mP->ai_next)
            {
                mSocket = socket(mP->ai_family, mP->ai_socktype, mP->ai_protocol);
                if (mSocket == -1)
                {
                    mSocket = 0;
                    cerr << "Connection::connect -- socket\n";
                    continue;
                }

                break;
            }
        }
    }

    void Connection::send(const void* inData, size_t inLength)
    {
        if (!mSocket)
        {
            cerr << "Connection::send -- invalid socket\n";
            return;
        }

        int numBytes = sendto(mSocket, inData, inLength, 0, mP->ai_addr,
            mP->ai_addrlen);

        if (numBytes == -1)
        {
            cerr << "Connection::send -- sendto\n";
            return;
        }

        cout << "sent " << numBytes << endl;
    }

    size_t Connection::receive(void* inBuffer, size_t inMaxLength)
    {
        sockaddr_storage their_addr;
        socklen_t addr_len = sizeof their_addr;
        int numBytes = recvfrom(mSocket, inBuffer, inMaxLength, 0,
            (sockaddr*)&their_addr, &addr_len);

        if (numBytes == -1)
        {
            cerr << "Connection::receive -- recvfrom\n";
            return 0;
        }

        if (numBytes > 0)
        {
            char s[INET6_ADDRSTRLEN];
            cout << "received packet from " << inet_ntop(their_addr.ss_family,
                get_in_addr((sockaddr*)&their_addr), s, sizeof s) << endl;
            cout << "packet is " << numBytes << " long" << endl;
            return numBytes;
        }

        return 0;
    }
}
