#include "FancyTestModule.h"
#include <XPG/Timer.hpp>
#include <XPG/OpenGL.hpp>

#define FOV 30.0f
#define NCC 1.0f
#define FCC 100.0f

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

GLfloat colors[32] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };

OGLI indices[36] = {
    0, 1, 2, 0, 2, 3, // top
    7, 6, 5, 7, 5, 4, // bottom
    1, 0, 4, 1, 4, 5, // right
    3, 2, 6, 3, 6, 7, // left
    2, 1, 5, 2, 5, 6, // front
    0, 3, 7, 0, 7, 4  // back
    };

GLfloat a = 1.0f / sqrt(3.0f);

GLfloat normals[24] = {
    a, a, a,
    a, -a, a,
    -a, -a, a,
    -a, a, a,
    a, a, -a,
    a, -a, -a,
    -a, -a, -a,
    -a, a, -a,
    };

FancyTestModule::FancyTestModule(int16u inMajorVersion) : mTS(1), mRotate(0.0f)
{
    mNextFrame = XPG::GetTicks();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

#ifndef XPG_PLATFORM_ANDROID
    mLegacy = inMajorVersion < 2;
#else
    mLegacy = false;
#endif

    if (mLegacy)
    {
        // no preparation
    }
    else
    {
        mVS.loadFromFile("test.vs", GL_VERTEX_SHADER);
        mFS.loadFromFile("test.fs", GL_FRAGMENT_SHADER);
        mProgram.attachShader(mVS);
        mProgram.attachShader(mFS);
        mProgram.bindAttribLocation(0, "in_Position");
        mProgram.bindAttribLocation(1, "in_Color");
        mProgram.bindAttribLocation(2, "in_Normal");
        mProgram.link();
        mUniMVPM = mProgram.getUniformLocation("MVPM");
        mUniMVM = mProgram.getUniformLocation("MVM");
        mUniNM = mProgram.getUniformLocation("NM");

        GLint u;
        vec4f v(0.1f, 0.1f, 0.1f, 1.0f);
        u = mProgram.getUniformLocation("ambientColor");
        glUniform4fv(u, 1, v.array());

        u = mProgram.getUniformLocation("diffuseColor");
        v[0] = 1.0f;
        v[1] = 1.0f;
        v[2] = 1.0f;
        glUniform4fv(u, 1, v.array());

        u = mProgram.getUniformLocation("specularColor");
        v[0] = 1.0f;
        v[1] = 1.0f;
        v[2] = 1.0f;
        v[3] = 0.0f;
        glUniform4fv(u, 1, v.array());

        u = mProgram.getUniformLocation("lightPosition");
        v[0] = 5.0f;
        v[2] = 100.0f;
        glUniform3fv(u, 1, v.array());

        mVBO.loadVAA(0, 3, 8, points);

        mVBO.loadVAA(1, 4, 8, colors);

        mVBO.loadVAA(2, 3, 8, normals);

        mIVBO.loadData(GL_TRIANGLES, 36, indices);

        glActiveTexture(GL_TEXTURE0);
        u = mProgram.getUniformLocation("cubeMap");
        glUniform1i(u, 0);
        XPG::Image img("test.png");
        XPG::Image* ip = &img;
        const XPG::Image* images[] = { ip, ip, ip, ip, ip, ip };
        mCubeMap.loadFromImages(images);
    }

    mCubeMap.bind();
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
}

FancyTestModule::~FancyTestModule()
{
    mProgram.clear();
}

void FancyTestModule::onExit()
{
    stopRunning();
}

void FancyTestModule::onKeyDown(XPG::Key::Code inKey)
{
    using namespace XPG;

    switch (inKey)
    {
        case Key::ESCAPE:
        {
            stopRunning();
            break;
        }

        default: {}
    }
}

void FancyTestModule::onResize(int32u inWidth, int32u inHeight)
{
    mWidth = inWidth;
    mHeight = inHeight;
    mRatio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(FOV, mRatio, NCC, FCC, true);
    mRange = NCC * tan(FOV * M_PI / 360.0f);
    mPixelRange = (inWidth > inHeight ? inHeight / 2 : inWidth / 2);

#ifndef XPG_PLATFORM_ANDROID
    if (mLegacy)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(mProjection);
        glMatrixMode(GL_MODELVIEW);
    }
#endif

    glViewport(0, 0, mWidth, mHeight);
}

void FancyTestModule::onDisplay()
{
    while (XPG::GetTicks() > mNextFrame)
    {
        mNextFrame += 25;
        mRotate += 1.0f;
        if (mRotate > 180.0f) mRotate -= 360.0f;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.loadIdentity();
    mModelView.translate(0.0f, 0.0f, -10.0f);
    mModelView.rotateX(mRotate);
    mModelView.rotateZ(mRotate);

#ifndef XPG_PLATFORM_ANDROID
    if (mLegacy)
    {
        glLoadMatrixf(mModelView);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, mTS.vertices());
        glColorPointer(4, GL_FLOAT, 0, mTS.colors());
        //glDrawElements(GL_TRIANGLES, 36, OGLIT, indices);
        glDrawArrays(GL_TRIANGLES, 0, mTS.size());

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else
#endif
    {
        mNormalView.loadIdentity();
        mNormalView.rotateX(mRotate);
        mNormalView.rotateZ(mRotate);

        mat4f mvp(mProjection, mModelView);
        glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, mvp);
        glUniformMatrix4fv(mUniMVM, 1, GL_FALSE, mModelView);
        glUniformMatrix4fv(mUniNM, 1, GL_FALSE, mNormalView);
        mVBO.display(mIVBO);
    }
}

void FancyTestModule::onMouseMove(int32u inX, int32u inY)
{
#ifndef XPG_PLATFORM_ANDROID
    //cout << inX << ' ' << inY << endl;
    int32s x = inX;
    int32s w = mWidth / 2;
    x -= w;
    float v = mRange * static_cast<float>(x) / static_cast<float>(mPixelRange);
    //cout << v;

    int32s y = inY;
    int32s h = mHeight / 2;
    y -= h;
    v = -mRange * static_cast<float>(y) / static_cast<float>(mPixelRange);
    //cout << ' ' << v << endl;
#endif
}

void FancyTestModule::handleEvent(const XPG::Event& inEvent)
{
    switch (inEvent.type)
    {
        case XPG::Event::WINDOW:
        {
            switch (inEvent.window.event)
            {
                case XPG::WindowEvent::EXIT:
                {
                    onExit();
                    break;
                }

                case XPG::WindowEvent::RESIZE:
                {
                    onResize(inEvent.window.width, inEvent.window.height);
                    break;
                }

                default:
                {

                }
            }

            break;
        }

        case XPG::Event::KEYBOARD:
        {
            onKeyDown(inEvent.keyboard.key);
            break;
        }

        case XPG::Event::MOUSE:
        {
            onMouseMove(inEvent.mouse.x, inEvent.mouse.y);
            break;
        }

        default:
        {

        }
    }
}
