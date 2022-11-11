//
// Created by chaofeng.li on 2022/1/12.
//

#ifndef FFMPEGPLAYER_XTHREAD_H
#define FFMPEGPLAYER_XTHREAD_H

void XSleep(int ms);

//c++11 线程库
class XThread {
public:
    virtual bool start();

    virtual void stop();

    virtual void pause(bool isPause);

    bool isPause() {
        isPausing = mIsPause;
        return mIsPause;
    }

    virtual void run() {}

protected:
    bool isExit = false;
    bool isRuning = false;
    bool mIsPause = false;
    bool isPausing = false;
private:
    void threadMain();
};


#endif //FFMPEGPLAYER_XTHREAD_H
