//
// Created by chaofeng.li on 2022/2/7.
//

#include "XShader.h"
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
//片元着色器
static const char *fragNV12 = GET_STR(
        precision mediump float; //
        varying vec2 vTextCoord;//顶点着色器传递的坐标
        uniform sampler2D yTextture;//输入的材质（不透明灰度图，单像素）
        uniform sampler2D uvTextture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTextture, vTextCoord).r;
            yuv.g = texture2D(uvTextture, vTextCoord).r - 0.5;
            yuv.b = texture2D(uvTextture, vTextCoord).a - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.5806, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);
//片元着色器
static const char *fragNV21 = GET_STR(
        precision mediump float; //
        varying vec2 vTextCoord;//顶点着色器传递的坐标
        uniform sampler2D yTextture;//输入的材质（不透明灰度图，单像素）
        uniform sampler2D uvTextture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTextture, vTextCoord).r;
            yuv.g = texture2D(uvTextture, vTextCoord).a - 0.5;
            yuv.b = texture2D(uvTextture, vTextCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.5806, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static GLuint initShared(const char *code, GLint type) {
    GLuint sh = glCreateShader(type);
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

void XShader::close() {
    mux.lock();
    //释放shader
    if (program)
        glDeleteProgram(program);
    if (fsh)
        glDeleteShader(fsh);
    if (vsh)
        glDeleteShader(vsh);
    //释放材质
    for (int i = 0; i < sizeof(texts) / sizeof(unsigned int); i++) {
        if (texts[i]) {
            glDeleteTextures(1, &texts[i]);
        }
        texts[i] = 0;
    }
    mux.unlock();
}

bool XShader::init(XShaderType type) {
    close();
    mux.lock();
    //shared初始化
    vsh = initShared(vertexShare, GL_VERTEX_SHADER);
    if (vsh == 0) {
        mux.unlock();
        return false;
    }
    switch (type) {
        case XSHADER_YUV420P:
            fsh = initShared(fragYUV420P, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:
            fsh = initShared(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fsh = initShared(fragNV21, GL_FRAGMENT_SHADER);
            break;
    }
    if (fsh == 0) {
        mux.unlock();
        return false;
    }
    //创建渲染程序
    program = glCreateProgram();
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        mux.unlock();
        LOGE("glLinkProgram failed");
        return false;
    }
    LOGD("glLinkProgram success");
    glUseProgram(program);

    //设置
    static float vers[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
    };
    GLuint apos = (GLuint) glGetAttribLocation(program, "aPostion");
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);

    static float txts[] = {
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
    };
    GLuint atxt = (GLuint) glGetAttribLocation(program, "aTextCoord");
    glEnableVertexAttribArray(atxt);
    glVertexAttribPointer(atxt, 2, GL_FLOAT, GL_FALSE, 8, txts);

    glUniform1i(glGetUniformLocation(program, "yTextture"), 0);
    switch (type) {
        case XSHADER_YUV420P:
            glUniform1i(glGetUniformLocation(program, "uTextture"), 1);
            glUniform1i(glGetUniformLocation(program, "vTextture"), 2);
            break;
        case XSHADER_NV12:
        case XSHADER_NV21:
            glUniform1i(glGetUniformLocation(program, "uvTextture"), 1);


    }
    glUniform1i(glGetUniformLocation(program, "uTextture"), 1);
    glUniform1i(glGetUniformLocation(program, "vTextture"), 2);
    mux.unlock();
    LOGD("init shader success");
    return true;
}

void XShader::draw() {
    LOGE("XShader draw");
    mux.lock();
    if (!program) {
        mux.unlock();
        LOGE("program is null");
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mux.unlock();
}

void
XShader::getTexture(unsigned int index, int width, int height, unsigned char *buff, bool isAlphe) {
    unsigned int format = GL_LUMINANCE;
    if (isAlphe) {
        format = GL_LUMINANCE_ALPHA;
    }
    mux.lock();
    if (texts[index] == 0) {
        glGenTextures(1, &texts[index]);

        glBindTexture(GL_TEXTURE_2D, texts[index]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //设置文理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,//GPU内部格式 亮度，灰度图
                     width, height,//尺寸要是2的次方
                     0,//边框
                     format,//数据的像素格式， 亮度，灰度图 要与上面一致
                     GL_UNSIGNED_BYTE,//像素的数据类型
                     NULL
        );
    }
    //纹理的修改和显示
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texts[index]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, buff);
    mux.unlock();
}