#ifndef __INPUT_H__
#define __INPUT_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;

class Input {
    virtual void onScaled(float factor);
    virtual void onLayerTouched(Ref *ref, Widget::TouchEventType type);
};

#endif /* __INPUT_H__ */