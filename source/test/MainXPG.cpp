#include <XPG/Main.hpp>
#include <XPG/Engine.hpp>
#include <XPG/Network/Connection.hpp>

#include "TestModule.hpp"

#include <iostream>
using namespace std;

void testEngine()
{
    XPG::Engine::Settings es;
    //es.legacyContext = true;
    es.fullScreen = true;
    es.context.vMajor = 3;
    es.context.vMinor = 2;

    XPG::Engine e(es);

    cout << "context version " << e.settings.context.vMajor << '.'
        << e.settings.context.vMinor << endl;

    cout << "shader version " << e.settings.shader.vMajor << '.'
        << e.settings.shader.vMinor << endl;

    {
        const char* title = e.settings.legacyContext ?
            "XPG OpenGL Legacy" : "XPG OpenGL 3";

        e.setWindowTitle(title);
        e.setIconTitle(title);

        TestModule tm;
        tm.onLoad();
        e.runModule(tm);
    }
}

void testServer(const char* inPort)
{
    XPG::Connection c;
    c.listen(inPort);

    char buffer[512] = "";
    if (c.receive(buffer, sizeof buffer))
    {
        buffer[511] = '\0';
        cout << "received data: " << buffer << endl;
    }
}

void testClient(const char* inPort, const char* inHost)
{
    XPG::Connection c;
    c.connect(inHost, inPort);

    char message[] = "Hello, Network!";
    c.send(message, sizeof message);
}

int MainXPG(int argc, char** argv)
{
    if (argc > 2)
    {
        testClient(argv[1], argv[2]);
    }
    else if (argc == 2)
    {
        testServer(argv[1]);
    }
    else
    {
        testEngine();
    }

    return 0;
}
