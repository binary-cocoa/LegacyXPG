#ifndef TESTMODULE_HPP
#define TESTMODULE_HPP

#include <XPG/Module.hpp>
#include <XPG/OpenGL/Shader.hpp>
#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/VertexBufferObject.hpp>

class TestModule : public XPG::Module
{
    public:
        TestModule();
        virtual ~TestModule();

        virtual void onDisplay();
        virtual void handleEvent(const XPG::Event& inEvent);

        void onResize(int32u inWidth, int32u inHeight);

    private:
        XPG::Program mProgram;
        XPG::Shader mVertexShader;
        XPG::Shader mFragmentShader;
        XPG::VertexBufferObject mVertexVBO;
        XPG::VertexBufferObject mColorVBO;
        XPG::VertexBufferObject mIndexVBO;

        int64u mNextFrame;
        GLint mUniMVPM;
        mat4f mProjection;
        mat4f mModelView;
        float mRotate;
};

#endif

