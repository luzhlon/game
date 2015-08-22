#ifndef __ROOMLISTSCENE_H__
#define __ROOMLISTSCENE_H__

#include "SceneLayer.h"

class RoomListScene : public SceneLayer
{
public:
    static Scene *createScene();
    class Item {
    public:
        static Widget *create(const string& str);
    };

    virtual bool init();

    CREATE_FUNC(RoomListScene);

    void onCreateClick(Ref *);
    void onEnterClick(Ref *);
    void onUpdateClick(Ref *);
    void onBackClick(Ref *);

    void onEnter() override;
    void onExit() override;

    void updateRoomList();

private:
    Button *m_btnCreate;
    Button *m_btnEnter;
    Button *m_btnLeave;
    ListView *m_listRoom;
};

#endif // ROOMLISTSCENE_H
