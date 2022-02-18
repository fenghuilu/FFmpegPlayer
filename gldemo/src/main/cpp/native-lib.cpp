#include <jni.h>
#include <stdio.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "LogCommon.h"
//顶点着色器
#define GET_STR(x) #x
static const char *vertexShare = GET_STR(
        attribute vec4 aPostion;//顶点坐标
        attribute vec2 aTextCoord;//材质坐标
        varying vec2 vTextCoord;//输出材质坐标
        void main() {
            vTextCoord = vec2(aTextCoord.x, 1.0 - aTextCoord.y);
            gl_Position = aPostion;
        }
);
//片元着色器
static const char *fragYUV420P = GET_STR(
        precision mediump float; //
        varying vec2 vTextCoord;//顶点着色器传递的坐标
        uniform sampler2D yTextture;//输入的材质（不透明灰度图，单像素）
        uniform sampler2D uTextture;
        uniform sampler2D vTextture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTextture, vTextCoord).r;
            yuv.g = texture2D(uTextture, vTextCoord).r - 0.5;
            yuv.b = texture2D(vTextture, vTextCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.5806, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

GLint initShared(const char *code, GLint type) {
    GLint sh = glCreateShader(type);
    if (sh == 0) {
        LOGE("glCreateShader failed");
        return 0;
    }
    glShaderSource(sh, 1, &code, 0);
    glCompileShader(sh);
    //获取编译结果
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGE("glCompileShader failed");
        return 0;
    }
    LOGD("initShared success ");

    return sh;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_feng_opengldemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT void JNICALL
Java_com_feng_opengldemo_XPlay_open(JNIEnv *env, jobject thiz, jstring url, jobject surface) {
    // TODO: implement open()
    const char* nurl = env->GetStringUTFChars(url,JNI_FALSE);
    FILE *fp = fopen(nurl, "rb");
    env->ReleaseStringUTFChars(url,nurl);
    if (!fp) {
        LOGD("fopen failed %s", url);
        return;
    }
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);
    //out
    EGLConfig config;
    EGLint configNum;
    //input
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    eglChooseConfig(display, configSpec, &config, 1, &configNum);
    EGLSurface winSurface = eglCreateWindowSurface(display, config, nwin, 0);
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    EGLContext eglContext = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    eglMakeCurrent(display, winSurface, winSurface, eglContext);
    LOGD("EGL init success");
    //shared初始化
    GLint vsh = initShared(vertexShare, GL_VERTEX_SHADER);
    GLint fsh = initShared(fragYUV420P, GL_FRAGMENT_SHADER);
    GLint program = glCreateProgram();
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        return;
    }
    LOGD("glLinkProgram success");

    glUseProgram(program);

    static float vers[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
    };
    GLuint apos = (GLuint) glGetAttribLocation(program, "aPostion");
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);

    static float txts[] = {
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };
    GLuint atxt = (GLuint) glGetAttribLocation(program, "aTextCoord");
    glEnableVertexAttribArray(atxt);
    glVertexAttribPointer(atxt, 2, GL_FLOAT, GL_FALSE, 8, txts);

    glUniform1i(glGetUniformLocation(program, "yTextture"), 0);
    glUniform1i(glGetUniformLocation(program, "uTextture"), 1);
    glUniform1i(glGetUniformLocation(program, "vTextture"), 2);
    GLuint texts[3] = {0};
    glGenTextures(3, texts);

    int width = 424;
    int height = 240;

    glBindTexture(GL_TEXTURE_2D, texts[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置文理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_LUMINANCE,//GPU内部格式 亮度，灰度图
                 width, height,//尺寸要是2的次方
                 0,//边框
                 GL_LUMINANCE,//数据的像素格式， 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,//像素的数据类型
                 NULL
    );
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置文理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_LUMINANCE,//GPU内部格式 亮度，灰度图
                 width / 2, height / 2,//尺寸要是2的次方
                 0,//边框
                 GL_LUMINANCE,//数据的像素格式， 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,//像素的数据类型
                 NULL
    );
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置文理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_LUMINANCE,//GPU内部格式 亮度，灰度图
                 width / 2, height / 2,//尺寸要是2的次方
                 0,//边框
                 GL_LUMINANCE,//数据的像素格式， 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,//像素的数据类型
                 NULL
    );

    unsigned char *buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];
    for (int i = 0; i < 10000; i++) {

        if (feof(fp) == 0) {
            fread(buf[0], 1, width * height, fp);
            fread(buf[1], 1, width * height/4, fp);
            fread(buf[2], 1, width * height/4, fp);
        }
//        memset(buf[0], i, width * height);
//        memset(buf[1], i, width * height / 4);
//        memset(buf[2], i, width * height / 4);

        //纹理的修改和显示
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texts[0]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texts[1]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texts[2]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[2]);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        eglSwapBuffers(display, winSurface);
    }
}