//
// Created by chaofeng.li on 2022/2/7.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include "XEGL.h"
#include "LogCommon.h"

class CXEGL : public XEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext eglContext = EGL_NO_CONTEXT;

    virtual bool init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *) win;
        //初始化EGL
        //
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            LOGE("eglGetDisplay failed");
            return false;
        }
        LOGD("eglGetDisplay success");
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            LOGE("eglInitialize failed");
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
            return false;
        }
        LOGD("eglCreateContext success");
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, eglContext)) {
            LOGE("eglMakeCurrent failed");
            return false;
        }
        LOGD("eglMakeCurrent success");
        return true;
    }
};

XEGL *XEGL::get() {
    static CXEGL egl;
    return &egl;
}