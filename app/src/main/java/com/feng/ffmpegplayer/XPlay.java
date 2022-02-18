package com.feng.ffmpegplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

/**
 * created by lichaofeng on 2022/2/7
 */
public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback {
    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
//        super.surfaceCreated(holder);
        //初始化EGL
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
}
