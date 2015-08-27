#ifndef __ROOMSCENE_H__
#define __ROOMSCENE_H__

#include "SceneLayer.h"

class RoomScene : public SceneLayer {
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(RoomScene);

    void onEnter() override;
private:
    //
};

#endif // __ROOMSCENE_H__
