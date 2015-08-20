#ifndef __ROOMSCENE_H__
#define __ROOMSCENE_H__

#include "cocos2d.h"
#include "HorseSoldier.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class RoomScene : public Layer {
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(RoomScene);
private:
    //
};

#endif // __ROOMSCENE_H__
