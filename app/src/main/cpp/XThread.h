//
// Created by chaofeng.li on 2022/1/12.
//

#ifndef FFMPEGPLAYER_XTHREAD_H
#define FFMPEGPLAYER_XTHREAD_H

//c++11 线程库
class XThread {
public:
    virtual void start();
    virtual void stop();
protected:
    virtual void run() = 0;
private:
    void threadMain();
};


#endif //FFMPEGPLAYER_XTHREAD_H
