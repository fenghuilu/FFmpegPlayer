//
// Created by chaofeng.li on 2022/2/7.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include "XEGL.h"
#include "LogCommon.h"
#include <mutex>

class CXEGL : public XEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext eglContext = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual bool init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *) win;
        //初始化EGL
        //
        close();
        mux.lock();
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            LOGE("eglGetDisplay failed");
            mux.unlock();
            return false;
        }
        LOGD("eglGetDisplay success");
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            LOGE("eglInitialize failed");
            mux.unlock();
            return false;
        }
        LOGD("eglInitialize success");
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfig = 0;
        if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfig)) {
            LOGE("eglChooseConfig failed");
            mux.unlock();
            return false;
        }
        LOGD("eglChooseConfig success");
        surface = eglCreateWindowSurface(display, config, nwin, NULL);
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        eglContext = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (eglContext == EGL_NO_CONTEXT) {
            LOGE("eglCreateContext failed");
            mux.unlock();
            return false;
        }
        LOGD("eglCreateContext success");
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, eglContext)) {
            LOGE("eglMakeCurrent failed");
            mux.unlock();
            return false;
        }
        LOGD("eglMakeCurrent success");
        mux.unlock();
        return true;
    }

    virtual void draw() {
        mux.lock();

        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mux.unlock();
            return;
        }
        eglSwapBuffers(display, surface);
        mux.unlock();
    }

    virtual void close() {
        mux.lock();
        if (display == EGL_NO_DISPLAY) {
            mux.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        if (eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(display, eglContext);
        }
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        eglContext = EGL_NO_CONTEXT;
        mux.unlock();
    }
};

XEGL *XEGL::get() {
    static CXEGL egl;
    return &egl;
}