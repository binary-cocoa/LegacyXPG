#include <XPG/Main.hpp>
#include <XPG/Engine.hpp>
#include <XPG/Image.hpp>

#include "TestModule.hpp"

#include <iostream>
using namespace std;

/// run test code
int MainXPG(int argc, char** argv)
{
    XPG::Engine::Settings es;
    es.legacyContext = true;
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

    return 0;
}
