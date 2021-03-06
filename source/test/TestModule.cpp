#include "TestModule.hpp"
#include <XPG/Timer.hpp>

TestModule::TestModule(XPG::Engine& inEngine) : mVertexShader(GL_VERTEX_SHADER),
    mFragmentShader(GL_FRAGMENT_SHADER),
    mIndexVBO(GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT), mEngine(inEngine),
    mRotate(0.0f)
{
}

void TestModule::onLoad()
{
    XPG::TimerStart();
    mNextFrame = XPG::GetTicks();

    static const char* vs =
        "uniform mat4 uMVPM;\n"
        "attribute vec3 iPosition;\n"
        "attribute vec4 iColor;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "  vColor = iColor;\n"
        "  gl_Position = uMVPM * vec4(iPosition, 1.0);\n"
        "}\n";

    mVertexShader.loadFromBuffer(vs);

    static const char* fs =
        "precision mediump float;\n"
        "uniform mat4 uCM;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "  gl_FragColor = uCM * vColor;\n"
        "}\n";

    mFragmentShader.loadFromBuffer(fs);

    mProgram.attachShader(mVertexShader);
    mProgram.attachShader(mFragmentShader);
    mProgram.bindAttribLocation(0, "iPosition");
    mProgram.bindAttribLocation(1, "iColor");
    mProgram.link();
    mUniMVPM = mProgram.getUniformLocation("uMVPM");
    mUniCM = mProgram.getUniformLocation("uCM");

    mat4f sepia;
    sepia(0, 0) = 0.5f;
    sepia(0, 1) = 0.4f;
    sepia(0, 2) = 0.2f;
    sepia(1, 0) = 0.4f;
    sepia(1, 1) = 0.3f;
    sepia(1, 2) = 0.2f;
    sepia(2, 0) = 0.3f;
    sepia(2, 1) = 0.3f;
    sepia(2, 2) = 0.2f;

    mat4f gray;
    for (size_t i = 0; i < 3; ++i)
    {
        gray(i, 0) = 0.3f;
        gray(i, 1) = 0.6f;
        gray(i, 2) = 0.1f;
    }

    //glUniformMatrix4fv(mUniCM, 1, GL_FALSE, gray);
    glUniformMatrix4fv(mUniCM, 1, GL_FALSE, mat4f());

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

    mColorVBO.loadData(colors, 8, 4);

    GLushort indices[36] = {
        0, 1, 2, 0, 2, 3, // top
        7, 6, 5, 7, 5, 4, // bottom
        1, 0, 4, 1, 4, 5, // right
        3, 2, 6, 3, 6, 7, // left
        2, 1, 5, 2, 5, 6, // front
        0, 3, 7, 0, 7, 4  // back
        };

    mIndexVBO.loadData(indices, 36);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
}

TestModule::~TestModule()
{
}

void TestModule::onDisplay()
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
    //mModelView.scaleX(3.0f);

    mat4f mvp(mProjection, mModelView);
    glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, mvp);

    mVertexVBO.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    mColorVBO.bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    mIndexVBO.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void TestModule::handleEvent(const XPG::Event& inEvent)
{
    switch (inEvent.type)
    {
        case XPG::Event::Window:
        {
            switch (inEvent.window.event)
            {
                case XPG::WindowEvent::Exit:
                {
                    stopRunning();
                    break;
                }

                case XPG::WindowEvent::Resize:
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

        case XPG::Event::Keyboard:
        {
            if (inEvent.keyboard.key == XPG::Key::Escape)
            {
                stopRunning();
            }
            else if (inEvent.keyboard.key == XPG::Key::Space
                && inEvent.keyboard.event == XPG::KeyboardEvent::Release)
            {
                mRotate = 0.0f;
            }
            else if (inEvent.keyboard.key == XPG::Key::F11
                && inEvent.keyboard.event == XPG::KeyboardEvent::Press)
            {
                //std::cerr << "herro\n";
                XPG::Fullscreen::Mode mode = XPG::Fullscreen::Soft;
                if (mEngine.settings().fullscreen == XPG::Fullscreen::Soft)
                    mode = XPG::Fullscreen::Off;

                mEngine.setFullscreen(mode);
            }
            break;
        }

        default:
        {

        }
    }
}

void TestModule::onResize(uint32 inWidth, uint32 inHeight)
{
    float ratio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(30.0f, ratio, 1.0f, 100.0f, true);
    glViewport(0, 0, inWidth, inHeight);
}
