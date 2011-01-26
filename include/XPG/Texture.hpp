#ifndef XPGH_TEXTURE
#define XPGH_TEXTURE

#include "OpenGL.hpp"

namespace XPG
{
    void setTexParams(GLenum inTarget, const GLenum inParameters[]);

    class Texture
    {
        public:
            Texture(GLenum inTarget);
            virtual ~Texture();

            inline void bind() { glBindTexture(mTarget, mHandle); }

        protected:
            GLenum mTarget;
            GLuint mHandle;
    };
}

#endif
