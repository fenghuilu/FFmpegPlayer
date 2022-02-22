package com.feng.ffmpegplayer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("ffmpegplayer");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d("haha","getExternalCacheDir().getAbsolutePath() "+getExternalCacheDir().getAbsolutePath());
        Log.d("haha","getDataDir().getAbsolutePath() "+getCacheDir().getAbsolutePath());
        open(getCacheDir().getAbsolutePath()+"/test.mp4");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String open(String url);
}