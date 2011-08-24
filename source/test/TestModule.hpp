#ifndef TESTMODULE_HPP
#define TESTMODULE_HPP

#include <XPG/Engine.hpp>
#include <XPG/OpenGL/Shader.hpp>
#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/VertexBufferObject.hpp>

#include "LiberateTypes.h"

class TestModule : public XPG::Module
{
    public:
        TestModule(XPG::Engine& inEngine);
        virtual ~TestModule();

        virtual void onLoad();
        virtual void onDisplay();
        virtual void handleEvent(const XPG::Event& inEvent);

        void onResize(uint32 inWidth, uint32 inHeight);

    private:
        XPG::Program mProgram;
        XPG::Shader mVertexShader;
        XPG::Shader mFragmentShader;
        XPG::VertexBufferObject mVertexVBO;
        XPG::VertexBufferObject mColorVBO;
        XPG::VertexBufferObject mIndexVBO;
        XPG::Engine& mEngine;

        uint64 mNextFrame;
        GLint mUniMVPM;
        GLint mUniCM;
        mat4f mProjection;
        mat4f mModelView;
        float mRotate;
};

#endif
