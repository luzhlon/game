#ifndef __ROOMSCENE_H__
#define __ROOMSCENE_H__

#include <string>
#include "SceneLayer.h"

using namespace std;

class RoomScene : public SceneLayer {
public:
    static string s_room_name;
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(RoomScene);

    void onReadyClick(Ref *);
    void onTeamClick(Ref *);

    void onEnter() override;
private:
    //
};

#endif // __ROOMSCENE_H__
