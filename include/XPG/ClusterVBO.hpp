#ifndef XPGH_CLUSTERVBO
#define XPGH_CLUSTERVBO

#include "IndexVBO.hpp"

namespace XPG
{
    template<size_t N>
    class ClusterVBO
    {
        public:
            ClusterVBO(GLenum inUsage = GL_STATIC_DRAW);
            ~ClusterVBO();

            void display(const IndexVBO& inIVBO) const;
            void display(GLenum inMode, GLint inFirst, GLsizei inCount) const;
            void setUsage(GLenum inUsage);
            void loadVAA(GLuint inVAI, GLuint inVPV, GLuint inSize,
                const GLfloat* inData);
            void editVAA(GLuint inVAI, GLuint inFirst, GLuint inSize,
                const GLfloat* inData);

        private:
            GLenum mUsage;
            GLuint mVBOI[N]; // vertex buffer objects indices
            GLuint mVPV[N]; // values per vertex
    };

    template<size_t N>
    ClusterVBO<N>::ClusterVBO(GLenum inUsage) : mUsage(inUsage)
    {
        glGenBuffers(N, mVBOI);
    }

    template<size_t N>
    ClusterVBO<N>::~ClusterVBO()
    {
        glDeleteBuffers(N, mVBOI);
    }

    template<size_t N>
    void ClusterVBO<N>::display(const IndexVBO& inIVBO) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVBOI[i]);
            glVertexAttribPointer(i, mVPV[i], GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(i);
        }

        inIVBO.draw();

        for (size_t i = 0; i < N; ++i)
            glDisableVertexAttribArray(i);
    }

    template<size_t N>
    void ClusterVBO<N>::display(GLenum inMode, GLint inFirst, GLsizei inCount)
        const
    {
        for (size_t i = 0; i < N; ++i)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVBOI[i]);
            glVertexAttribPointer(i, mVPV[i], GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(i);
        }

        glDrawArrays(inMode, inFirst, inCount);

        for (size_t i = 0; i < N; ++i)
            glDisableVertexAttribArray(i);
    }

    template<size_t N>
    void ClusterVBO<N>::setUsage(GLenum inUsage)
    {
        mUsage = inUsage;
    }

    template<size_t N>
    void ClusterVBO<N>::loadVAA(GLuint inVAI, GLuint inVPV, GLuint inSize,
        const GLfloat* inData)
    {
        mVPV[inVAI] = inVPV;
        glBindBuffer(GL_ARRAY_BUFFER, mVBOI[inVAI]);
        glBufferData(GL_ARRAY_BUFFER, inSize * sizeof(GLfloat) * mVPV[inVAI],
            inData, mUsage);
    }

    template<size_t N>
    void ClusterVBO<N>::editVAA(GLuint inVAI, GLuint inFirst, GLuint inSize,
        const GLfloat* inData)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVBOI[inVAI]);
        glBufferSubData(GL_ARRAY_BUFFER, inFirst * sizeof(GLfloat)
            * mVPV[inVAI], inSize * sizeof(GLfloat) * mVPV[inVAI], inData);
    }
}

#endif

