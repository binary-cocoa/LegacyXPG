#include <XPG/Network/Connection.hpp>
#include <sstream>
#include <iostream>
using namespace std;

namespace XPG
{
    Connection::Connection() : mSocket(0)
    {
    }

    Connection::~Connection()
    {
    }

    void Connection::listen(int16u inPort)
    {
        if (!mSocket)
        {
            for (mP = mServInfo; mP; mP = mP->ai_next)
            {
                mSocket = socket(mP->ai_family, mP->ai_socktype, mP->ai_protocol);
                if (mSocket == -1)
                {
                    cerr << "Connection::connect -- socket\n";
                    continue;
                }

                break;
            }
        }
    }

    void Connection::connect(const char* inAddress, int16u inPort)
    {
        stringstream ss;
        ss << inPort;

        if (getaddrinfo(inAddress, ss.str().c_str(), &mHints, &mServInfo))
        {
            cerr << "Connection::connect -- getaddrinfo\n";
        }

        if (!mSocket)
        {
            for (mP = mServInfo; mP; mP = mP->ai_next)
            {
                mSocket = socket(mP->ai_family, mP->ai_socktype, mP->ai_protocol);
                if (mSocket == -1)
                {
                    cerr << "Connection::connect -- socket\n";
                    continue;
                }
            }
        }
    }

    void Connection::send(const void* inData, size_t inLength)
    {
        if (sendto(mSocket, inData, inLength, 0, ))
    }

    bool Connection::receive(int8u* inBuffer)
    {
        return false;
    }
}
