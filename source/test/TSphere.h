#ifndef TSPHERE_H
#define TSPHERE_H

#include <XPG/OpenGL.hpp>

class TSphere
{
    public:
        TSphere(size_t inDetail = 0);
        ~TSphere();
        
        inline const size_t   size()     const { return mNumVertices; }
        inline const GLfloat* vertices() const { return mVertices;    }
        inline const GLfloat* normals()  const { return mNormals;     }
        inline const GLfloat* colors()   const { return mColors;      }

    private:
        void normalize(GLfloat inVector[3]);
        void addVertex(const GLfloat inVertex[3], const GLfloat inColor[4]);
        void addTriangle(const GLfloat inA[3], const GLfloat inB[3],
            const GLfloat inC[3]);
        void subdivide(const GLfloat inA[3], const GLfloat inB[3],
            const GLfloat inC[3],
            size_t inDepth);
        
        size_t mIndex;
        size_t mNumVertices;
        GLfloat* mVertices;
        GLfloat* mNormals;
        GLfloat* mColors;
};

#endif
