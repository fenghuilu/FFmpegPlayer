package com.feng.opengldemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

/**
 * created by lichaofeng on 2022/2/9
 */
public class XPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback {

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void run() {
        open("/storage/emulated/0/Android/data/com.feng.opengldemo/cache/out.yuv",getHolder().getSurface());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        new Thread(this).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }
    public native void open(String url,Object surface);
}
