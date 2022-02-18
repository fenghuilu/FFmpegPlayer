//
// Created by chaofeng.li on 2022/1/12.
//

#ifndef FFMPEGPLAYER_XTHREAD_H
#define FFMPEGPLAYER_XTHREAD_H

void XSleep(int ms);

//c++11 线程库
class XThread {
public:
<<<<<<< HEAD
    virtual void start();
    virtual void stop();
<<<<<<< HEAD
protected:
    virtual void run() = 0;
=======
    virtual void run(){}
>>>>>>> first commit
=======
    void start();

    void stop();

    virtual void run() {}

protected:
    bool isExit = false;
    bool isRuning = false;
>>>>>>> add gldemo
private:
    void threadMain();
};


#endif //FFMPEGPLAYER_XTHREAD_H
