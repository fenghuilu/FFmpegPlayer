//
// Created by chaofeng.li on 2022/1/12.
//

#include "XThread.h"
#include <thread>
#include "LogCommon.h"

#define LOG_TAG "XThread"
using namespace std;

void XSleep(int ms) {
    chrono::milliseconds du(ms);
    this_thread::sleep_for(du);
}

void XThread::start() {
    isExit = false;
    thread th(&XThread::threadMain, this);
    th.detach();
}

void XThread::stop() {
    isExit = true;
    for(int i =0;i<200;i++){
        if(!isRuning){
            LOGD("stop 停止线程成功");
            return;
        }
        XSleep(1);
    }
    LOGD("stop 停止线程超时");
}

void XThread::threadMain() {
    isRuning = true;
    LOGD("线程函数进入");
    run();
    LOGD("线程函数退出");
    isRuning = false;
}