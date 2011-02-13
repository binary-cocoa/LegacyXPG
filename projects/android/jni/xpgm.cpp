#include <jni.h>

#include <TestModule.hpp>

static XPG::Module* theModule = NULL;

extern "C"
{
    JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onLoad(JNIEnv* env,
        jobject obj);
    JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onResize(JNIEnv* env,
        jobject obj, jint inWidth, jint inHeight);
    JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onLoop(JNIEnv* env,
        jobject obj);
};

JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onLoad(JNIEnv* env,
    jobject obj)
{
    XPG::newContext();
    if (!theModule) theModule = new TestModule;
    theModule->onLoad();
}

JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onResize(JNIEnv * env,
    jobject obj, jint inWidth, jint inHeight)
{
    XPG::Event event;
    event.type = XPG::Event::WINDOW;
    event.window.event = XPG::WindowEvent::RESIZE;
    event.window.width = inWidth;
    event.window.height = inHeight;
    theModule->handleEvent(event);
}

JNIEXPORT void JNICALL Java_xpg_mobile_AccessJNI_onLoop(JNIEnv* env,
    jobject obj)
{
    theModule->onDisplay();
}

