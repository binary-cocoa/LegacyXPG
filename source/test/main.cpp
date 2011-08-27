#include <XPG/Engine.hpp>
#include <XPG/Image.hpp>

#include "TestModule.hpp"
#include "TestGL32Module.hpp"

#include <iostream>
using namespace std;

#define GL_VERIFY(n) if (!n) cerr << "null ref to " << #n << '\n'

/// run test code
int main(int argc, char** argv)
{
    XPG::Engine::Settings es;
    //es.fullscreen = XPG::Fullscreen::Soft;
    es.profile = XPG::Context::Legacy;
    es.profile = XPG::Context::GL32;

    XPG::Engine e(es);

    cout << "context version " << e.settings().context.vMajor << '.'
        << e.settings().context.vMinor << endl;

    cout << "shader version " << e.settings().shader.vMajor << '.'
        << e.settings().shader.vMinor << endl;

    {
        const char* title = e.settings().profile == XPG::Context::Legacy ?
            "XPG OpenGL Legacy" : "XPG OpenGL 3";

        e.setWindowTitle(title);
        e.setIconTitle(title);

        GL_VERIFY(glVertexAttribPointer);
        GL_VERIFY(glDrawElements);
        GL_VERIFY(glGenBuffers);
        GL_VERIFY(glGenerateMipmap);

        if (e.settings().profile == XPG::Context::Legacy)
        {
            TestModule tm(e);
            tm.onLoad();
            e.runModule(tm);
        }
        else
        {
            TestGL32Module tm(e);
            e.runModule(tm);
        }
    }

    return 0;
}
