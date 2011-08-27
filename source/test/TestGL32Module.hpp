#ifndef TESTGL32MODULE_H
#define TESTGL32MODULE_H

#include <XPG/Engine.hpp>
#include <XPG/OpenGL/VertexBufferObject.hpp>
#include <XPG/OpenGL/VertexShader.hpp>
#include <XPG/OpenGL/FragmentShader.hpp>
#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/ShortIndexVBO.hpp>

#include "SmartModule.hpp"

class TestGL32Module : public SmartModule
{
    public:
        TestGL32Module(XPG::Engine& inEngine);
        virtual ~TestGL32Module();

        virtual void onDisplay();
        void onUpdate();

        virtual void onKeyDown(XPG::Key::Code inKey);

        virtual void onResize(uint32 inWidth, uint32 inHeight);

    protected:
    private:
        XPG::Engine& mEngine;
        XPG::VertexShader mVertexShader;
        XPG::FragmentShader mFragmentShader;
        XPG::Program mProgram;
        XPG::VertexBufferObject mVertexVBO;
        XPG::VertexBufferObject mColorVBO;
        XPG::ShortIndexVBO mIndexVBO;

        GLint mUniformMatrix;

        mat4f mProjectionMatrix;
        mat4f mModelViewMatrix;
        mat4f mModelViewProjectionMatrix;

        float mRotate;
        uint64 mNextFrame;
};

#endif
