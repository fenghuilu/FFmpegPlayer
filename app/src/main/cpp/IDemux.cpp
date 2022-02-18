//
// Created by chaofeng.li on 2022/1/11.
//

#include "IDemux.h"
#include "LogCommon.h"

void IDemux::run() {
    while (!isExit) {
        XData d = read();
        if (d.size > 0) {
            notify(d);
        } else {
            break;
        }
    }
}
