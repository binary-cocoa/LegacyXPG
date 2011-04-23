#ifndef XPGH_CONNECTION
#define XPGH_CONNECTION

#include "../DataTypes.hpp"
#include "LowLevel.hpp"

namespace XPG
{
    class Connection
    {
    public:
        Connection();
        virtual ~Connection();

        void listen(int16u inPort);
        void connect(const char* inAddress, int16u inPort);
        void send(const void* inData, size_t inLength);
        bool receive(int8u* inBuffer);

    private:
        Connection(const Connection& inConnection)
        {
            // no copying
        }

        Connection& operator=(const Connection& inConnection)
        {
            // no copying
            return *this;
        }

        SOCKET mSocket;
        addrinfo mHints;
        addrinfo* mServInfo;
        addrinfo* mP;
    };
}

#endif

