#ifndef XPGH_TEXTURE
#define XPGH_TEXTURE

#include "OpenGL.hpp"

namespace XPG
{
    class Texture
    {
        public:
            Texture(GLenum inTarget);
            virtual ~Texture();

            inline void bind() { glBindTexture(mTarget, mHandle); }

        protected:
            void setTexParams(const GLenum inParameters[]);

            GLenum mTarget;

        private:
            GLuint mHandle;
    };
}

#endif
