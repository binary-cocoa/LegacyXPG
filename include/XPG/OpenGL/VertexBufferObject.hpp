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
            virtual ~VertexBufferObject();

            inline void bind() const { glBindBuffer(mTarget, mHandle); }
            void loadData(const GLvoid* inData, GLuint inSize,
                GLuint inValuesPerUnit = 1);
            void editData(const GLvoid* inData, GLuint inFirst,
                GLuint inVertexCount = 1); // untested
            void enableVAA(GLuint inIndex);
            void disableVAA();

        protected:
            GLuint mHandle;
            GLuint mIndex;
            GLenum mTarget;
            GLenum mType;
            GLenum mUsage;
            GLuint mTypeSize;
            GLuint mValuesPerUnit;
            GLuint mSize;
    };
}

#endif
