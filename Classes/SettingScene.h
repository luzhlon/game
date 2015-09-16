#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__

#include "SceneLayer.h"

class SettingScene : public SceneLayer {
public:
    static Scene* createScene();
    CREATE_FUNC(SettingScene);
    virtual bool init();
    void onOkClicked(Ref *);
    void onCancelClicked(Ref *);
    void saveSettings();
private:
    TextField *m_textIP;
    Slider    *m_slidVolum;
};

#endif //__MENUSCENE_H__
