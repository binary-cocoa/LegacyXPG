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
        void listen(const char* inPort);
        void connect(const char* inAddress, int16u inPort);
        void connect(const char* inAddress, const char* inPort);
        void send(const void* inData, size_t inLength);
        size_t receive(void* inBuffer, size_t inMaxLength);

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
        struct addrinfo mHints;
        struct addrinfo* mServInfo;
        struct addrinfo* mP;
    };
}

#endif

