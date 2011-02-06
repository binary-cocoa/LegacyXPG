#ifndef TESTMODULE_HPP
#define TESTMODULE_HPP

#include <XPG/Module.hpp>
#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/VertexBufferObject.hpp>

class TestModule : public XPG::Module
{
    public:
        TestModule();
        virtual ~TestModule();

        virtual void onDisplay();        
        virtual void handleEvent(const XPG::Event& inEvent);
        
    private:
}

#endif
