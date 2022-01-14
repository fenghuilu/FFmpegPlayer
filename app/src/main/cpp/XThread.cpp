//
// Created by chaofeng.li on 2022/1/12.
//

#include "XThread.h"
#include <thread>
#include "LogCommon.h"
#define LOG_TAG "XThread"
using namespace std;

void XThread::start() {
    thread th(&XThread::threadMain,this);
    th.detach();
}

void XThread::stop() {

}

void XThread::threadMain() {
    LOGD("线程函数进入");
    run();
    LOGD("线程函数退出");
}