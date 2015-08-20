#ifndef __ROOMLISTSCENE_H__
#define __ROOMLISTSCENE_H__

#include "SceneLayer.h"

class RoomListScene : public SceneLayer
{
public:
    static Scene *createScene();

    virtual bool init();

    CREATE_FUNC(RoomListScene);

    void onCreateClick(Ref *);
    void onEnterClick(Ref *);
    void onLeaveClick(Ref *);

    void onEnter();

private:
    Button *m_btnCreate;
    Button *m_btnEnter;
    Button *m_btnLeave;
    ListView *m_listRoom;
};

#endif // ROOMLISTSCENE_H
