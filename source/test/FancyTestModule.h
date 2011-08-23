#ifndef FANCYTESTMODULE_H
#define FANCYTESTMODULE_H

#include <XPG/Module.hpp>
#include <XPG/OpenGL/Shader.hpp>
#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/CubeMap.hpp>
#include <XPG/OpenGL/VertexArrayObject.hpp>

#include "TSphere.h"
#include "LiberateTypes.h"

class FancyTestModule : public XPG::Module
{
    public:
        FancyTestModule(uint16 inMajorVersion);
        virtual ~FancyTestModule();

        void onExit();
        void onKeyDown(XPG::Key::Code inKey);
        void onMouseMove(uint32 inX, uint32 inY);
        void onResize(uint32 inWidth, uint32 inHeight);

        virtual void handleEvent(const XPG::Event& inEvent);
        virtual void onDisplay();

    private:
        XPG::Program mProgram;
        XPG::Shader mVS;
        XPG::Shader mFS;
        XPG::CubeMap mCubeMap;
        XPG::VertexArrayObject mVAO;
        XPG::VertexBufferObject mVertexVBO;
        XPG::VertexBufferObject mColorVBO;
        XPG::VertexBufferObject mNormalVBO;
        XPG::VertexBufferObject mIndexVBO;

        TSphere mTS;
        GLint mUniMVPM;
        GLint mUniMVM;
        GLint mUniNM;
        mat4f mProjection;
        mat4f mModelView;
        mat4f mNormalView;
        float mRange;
        float mRatio;
        float mRotate;
        uint32 mWidth;
        uint32 mHeight;
        uint32 mPixelRange;
        uint64 mNextFrame;
        bool mLegacy;
};

#endif
