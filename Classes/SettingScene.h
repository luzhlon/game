#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__

#include "SceneLayer.h"

class SettingScene : public SceneLayer {
public:
    static Scene* createScene();
    CREATE_FUNC(SettingScene);
    virtual bool init();
    void onOkClicked(Ref *);

    void onEnter() override;
private:
    TextField *m_textIP;
    TextField *m_textPort;
};

#endif //__MENUSCENE_H__
