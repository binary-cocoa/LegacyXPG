#include "TestGL32Module.hpp"

#include <XPG/Timer.hpp>

TestGL32Module::TestGL32Module(XPG::Engine& inEngine) : mEngine(inEngine),
    mRotate(0.0f)
{
    const char* vs =
        "#version 150\n"

        "uniform mat4 uMVPM;\n"

        "in vec3 iVertex;\n"
        "in vec3 iColor;\n"

        "out vec3 vColor;"

        "void main()\n"
        "{\n"
        "    const float pi = 3.1415926535898;\n"
        "    vColor = iColor;\n"
        "    vec4 v = vec4(iVertex, 1.0);\n"
        "    float i = float(gl_InstanceID);\n"
        "    v.x += sin(pi * i / 8) * i / 2.0;\n"
        "    v.y += cos(pi * i / 8) * i / 2.0;\n"
        //"    v.z += 2.5 * i;\n"
        "    gl_Position = uMVPM * v;\n"
        "}\n"
        ;

    const char* fs =
        "#version 150\n"

        "in vec3 vColor;\n"

        "out vec4 oColor;\n"

        "void main()\n"
        "{\n"
        "    oColor = vec4(vColor, 1.0);\n"
        "}\n"
        ;

    mVertexShader.loadFromBuffer(vs);
    mFragmentShader.loadFromBuffer(fs);
    mProgram.attachShader(mVertexShader);
    mProgram.attachShader(mFragmentShader);
    mProgram.bindAttribLocation(0, "iVertex");
    mProgram.bindAttribLocation(1, "iColor");
    mProgram.link();
    mUniformMatrix = mProgram.getUniformLocation("uMVPM");

    GLfloat points[24] = {
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f
        };

    mVertexVBO.loadData(points, 8, 3);

    for (size_t i = 0; i < 24; ++i)
        points[i] = (points[i] + 1.0f) / 2.0f;

    mColorVBO.loadData(points, 8, 3);

    GLushort indices[36] = {
        0, 1, 2, 0, 2, 3, // top
        7, 6, 5, 7, 5, 4, // bottom
        1, 0, 4, 1, 4, 5, // right
        3, 2, 6, 3, 6, 7, // left
        2, 1, 5, 2, 5, 6, // front
        0, 3, 7, 0, 7, 4  // back
        };

    mIndexVBO.loadData(indices, 12, 3);

    //mModelViewMatrix.translate(0.0f, 0.0f, -10.0f);

    mNextFrame = XPG::GetTicks();

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
}

TestGL32Module::~TestGL32Module()
{
    glDisable(GL_DEPTH_TEST);
}

void TestGL32Module::onUpdate()
{
    mRotate += 1.0f;
    if (mRotate > 180.0f) mRotate -= 360.0f;

    mModelViewMatrix.loadIdentity();
    mModelViewMatrix.translate(0.0f, 0.0f, -40.0f);
    mModelViewMatrix.rotateX(mRotate);
    mModelViewMatrix.rotateY(mRotate);

    mModelViewProjectionMatrix.multiply(mProjectionMatrix, mModelViewMatrix);
}

void TestGL32Module::onDisplay()
{
    while (XPG::GetTicks() > mNextFrame)
    {
        mNextFrame += 25;
        onUpdate();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(mUniformMatrix, 1, GL_FALSE, mModelViewProjectionMatrix);
    mVertexVBO.enableVAA(0);
    mColorVBO.enableVAA(1);
    mIndexVBO.drawInstanced(300);
    mColorVBO.disableVAA();
    mVertexVBO.disableVAA();
}

void TestGL32Module::onKeyDown(XPG::Key::Code inKey)
{
    switch (inKey)
    {
        case XPG::Key::Escape:
        {
            stopRunning();
            break;
        }

        case XPG::Key::F11:
        {
            if (mEngine.settings().fullscreen == XPG::Fullscreen::Off)
                mEngine.setFullscreen(XPG::Fullscreen::Soft);
            else
                mEngine.setFullscreen(XPG::Fullscreen::Off);

            break;
        }

        default: {}
    }
}

void TestGL32Module::onResize(uint32 inWidth, uint32 inHeight)
{
    float ratio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
    mProjectionMatrix.loadIdentity();
    mProjectionMatrix.perspective(30.0f, ratio, 1.0f, 1000.0f, true);
    mModelViewProjectionMatrix.multiply(mProjectionMatrix, mModelViewMatrix);

    glViewport(0, 0, inWidth, inHeight);
}
