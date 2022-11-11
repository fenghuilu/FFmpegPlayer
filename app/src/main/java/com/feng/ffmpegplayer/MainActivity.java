package com.feng.ffmpegplayer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatSeekBar;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements Runnable {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("ffmpegplayer");
    }

    AppCompatSeekBar mSeekBar;
    private Thread thread;

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
        mSeekBar.setMax(1000);
        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {

                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seek((double) seekBar.getProgress() / seekBar.getMax());
            }
        });
        thread = new Thread(this);
        thread.start();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String open(String url);

    public native double getProgress();

    public native boolean seek(double pos);

    @Override
    public void run() {
        for (; ; ) {
            mSeekBar.setProgress((int) (getProgress() * 1000));
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}