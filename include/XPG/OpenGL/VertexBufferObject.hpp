#ifndef XPGH_VERTEXBUFFEROBJECT
#define XPGH_VERTEXBUFFEROBJECT

#include "Base.hpp"
#include "../Platforms.hpp"

namespace XPG
{
    class VertexBufferObject
    {
        public:
#ifndef XPG_OPENGL_ES
            friend class VertexArrayObject;
#endif

            VertexBufferObject(GLenum inTarget = GL_ARRAY_BUFFER,
                GLenum inType = GL_FLOAT, GLenum inUsage = GL_STATIC_DRAW);
            ~VertexBufferObject();

            inline void bind() const { glBindBuffer(mTarget, mHandle); }
            void loadData(const GLvoid* inData, GLuint inSize,
                GLuint inValuesPerUnit = 1);

        private:
            void create();

            GLuint mHandle;
            GLenum mTarget;
            GLenum mType;
            GLenum mUsage;
            GLuint mTypeSize;
            GLuint mValuesPerUnit;
            int16u mContextVersion;
    };
}

#endif
