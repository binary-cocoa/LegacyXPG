#include "TSphere.h"

#include <cassert>
#include <cmath>

#define IX 0.525731112119133606f
#define IZ 0.850650808352039932f

const GLfloat TSVD[12][3] =
    {{-IX, 0.0f, IZ}, {IX, 0.0f, IZ}, {-IX, 0.0f, -IZ}, {IX, 0.0f, -IZ},
     {0.0f, IZ, IX}, {0.0f, IZ, -IX}, {0.0f, -IZ, IX}, {0.0f, -IZ, -IX},
     {IZ, IX, 0.0f}, {-IZ, IX, 0.0f}, {IZ, -IX, 0.0f}, {-IZ, -IX, 0.0f}};
const GLushort TSTI[20][3] =
    {{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
     {1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
     {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
     {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}};

TSphere::TSphere(size_t inDetail) : mIndex(0), mVertices(NULL), mNormals(NULL),
    mColors(NULL)
{
    mNumVertices = 20 * 3;

    for (size_t i = 0; i < inDetail; ++i)
        mNumVertices *= 4;

    mVertices = new GLfloat[mNumVertices * 3];
    mColors = new GLfloat[mNumVertices * 4];

    for (size_t i = 0; i < 20; ++i)
        subdivide(TSVD[TSTI[i][2]], TSVD[TSTI[i][1]], TSVD[TSTI[i][0]],
            inDetail);
}

TSphere::~TSphere()
{
    if (mVertices) delete [] mVertices;
    if (mNormals)  delete [] mNormals;
    if (mColors)   delete [] mColors;
}

void TSphere::normalize(GLfloat inVector[3])
{
    float length = sqrt(inVector[0] * inVector[0] + inVector[1] * inVector[1]
        + inVector[2] * inVector[2]);

    for (size_t i = 0; i < 3; ++i) inVector[i] /= length;
}

void TSphere::addVertex(const GLfloat inVertex[3], const GLfloat inColor[4])
{
    for (size_t i = 0; i < 3; ++i)
    {
        mVertices[mIndex * 3 + i] = inVertex[i];
        mColors[mIndex * 4 + i] = inColor[i];
    }

    assert(mIndex < mNumVertices);

    ++mIndex;
}

void TSphere::addTriangle(const GLfloat inA[3], const GLfloat inB[3],
    const GLfloat inC[3])
{
    const GLfloat R[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat G[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    const GLfloat B[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    addVertex(inA, R);
    addVertex(inC, G);
    addVertex(inB, B);
}

void TSphere::subdivide(const GLfloat inA[3], const GLfloat inB[3],
    const GLfloat inC[3], size_t inDepth)
{
    if (!inDepth)
    {
        addTriangle(inA, inB, inC);
        return;
    }

    GLfloat ab[3];
    GLfloat bc[3];
    GLfloat ca[3];

    for (size_t i = 0; i < 3; ++i)
    {
        ab[i] = (inA[i] + inB[i]) / 2.0f;
        bc[i] = (inB[i] + inC[i]) / 2.0f;
        ca[i] = (inC[i] + inA[i]) / 2.0f;
    }

    normalize(ab);
    normalize(bc);
    normalize(ca);

    subdivide(inA, ab, ca, inDepth - 1);
    subdivide(inB, bc, ab, inDepth - 1);
    subdivide(inC, ca, bc, inDepth - 1);
    subdivide(ab, bc, ca, inDepth - 1);

}
