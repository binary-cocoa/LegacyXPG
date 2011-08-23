#include <XPG/OpenGL/Base.hpp>

#define CHECK_ENUM(n) case n: return #n

namespace XPG
{
    const char* OpenGLErrorString()
    {
        switch (glGetError())
        {
            CHECK_ENUM(GL_INVALID_VALUE);
            CHECK_ENUM(GL_INVALID_ENUM);
            CHECK_ENUM(GL_INVALID_OPERATION);
            CHECK_ENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
            CHECK_ENUM(GL_OUT_OF_MEMORY);
            CHECK_ENUM(GL_NO_ERROR);

            default: return "none";
        }
    }

    namespace OpenGL
    {
        uint32 Context::mMasterVersion = 1;

        void Context::destroy()
        {
            ++mMasterVersion;
        }

        Context::Context() : mVersion(0)
        {
        }

        bool Context::isOutdated() const
        {
            return mVersion < mMasterVersion;
        }

        void Context::update()
        {
            mVersion = mMasterVersion;
        }
    }
}
