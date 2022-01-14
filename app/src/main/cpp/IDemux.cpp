//
// Created by chaofeng.li on 2022/1/11.
//

#include "IDemux.h"
#include "LogCommon.h"

void IDemux::run() {
    for(;;){
        XData d = read();
        LOGD("IDemux read %d",d.size);
        if(d.size<=0){
            break;
        }
    }
}
