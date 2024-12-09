#ifndef __PAUSA_SCENE_H__
#define __PAUSA_SCENE_H__

#include "cocos2d.h"

class pausa : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene(cocos2d::Scene* escenaFondo);
    virtual bool init();

    CREATE_FUNC(pausa);

private:
    void menuReanudarCallback(cocos2d::Ref* pSender);
    void menuReintentarCallback(cocos2d::Ref* pSender);
    void menuPrincipalCallback(cocos2d::Ref* pSender);

    cocos2d::Scene* escenaAnterior;
};

#endif // __PAUSA_SCENE_H__
