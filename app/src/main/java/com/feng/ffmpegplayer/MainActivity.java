package com.feng.ffmpegplayer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatSeekBar;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("ffmpegplayer");
    }

    AppCompatSeekBar mSeekBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d("haha", "MainActivity onCreate" + getCacheDir().getPath());

        findViewById(R.id.openLOCAL).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                open("/sdcard/Android/data/com.feng.ffmpegplayer/cache/test.mp4");
            }
        });
        findViewById(R.id.openRTSP).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                open("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mp4");
            }
        });
        mSeekBar = findViewById(R.id.seek_bar);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String open(String url);
}