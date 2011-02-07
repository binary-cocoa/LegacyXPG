#include <XPG/Display.hpp>
#include <XPG/Image.hpp>

#include "TestModule.hpp"

#include <iostream>
using namespace std;

/// run test code
int main(int argc, char** argv)
{
    XPG::Context::Parameters p;
    //p.legacyContext = true;
    p.context.vMajor = 3;
    p.context.vMinor = 2;

    XPG::Context c;
    c.create(p);

    cout << "context version " << c.details.context.vMajor << '.'
        << c.details.context.vMinor << endl;

    cout << "shader version " << c.details.shader.vMajor << '.'
        << c.details.shader.vMinor << endl;

    {
        const char* title = c.details.legacyContext ?
            "XPG OpenGL Legacy" : "XPG OpenGL 3";

        c.setWindowTitle(title);
        c.setIconTitle(title);

        TestModule tm;
        c.runModule(tm);
    }

    c.destroy();

    return 0;
}
