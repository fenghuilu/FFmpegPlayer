package com.feng.ffmpegplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * created by lichaofeng on 2022/2/7
 */
public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback ,GLSurfaceView.Renderer{
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        setRenderer(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        super.surfaceCreated(holder);
        //初始化EGL
        Log.d("haha","surfaceCreated");
//        setRenderer(this);
        initView(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
//        super.surfaceDestroyed(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
//        super.surfaceChanged(holder, format, w, h);
    }

    public native void initView(Object surface);

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }
}
