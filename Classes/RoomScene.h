#ifndef __ROOMSCENE_H__
#define __ROOMSCENE_H__

#include "Client.h"
#include "SceneLayer.h"

class RoomScene : public SceneLayer {
public:
    static char s_room_name[MAX_ROOM_NAME_LEN];
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(RoomScene);

    void onReadyClick(Ref *);
    void onTeamClick(Ref *);
    void load_layouts(Layout *);
    void set_member_info(int index, room_member *meb);

    void onEnter() override;
private:
    //
    Layout *_layout_member[MAX_ROOM_MEMBERS];
};

#endif // __ROOMSCENE_H__
