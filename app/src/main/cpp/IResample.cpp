//
// Created by Chaofeng.Li on 2022/9/30.
//

#include "IResample.h"
void IResample::update(XData indata){
    XData d = this->resample(indata);
    if(d.size >0){
        this->notify(d);
    }
}