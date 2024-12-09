#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(int mapaActual); // M�todo para crear la escena con el mapa actual
    virtual bool init(); // Inicializaci�n b�sica
    bool initWithMapa(int mapaActual); // Inicializaci�n personalizada con el mapa actual
    CREATE_FUNC(GameOverScene);

private:
    int mapaActual_; // Variable para almacenar el mapa actual
};

#endif // __GAME_OVER_SCENE_H__



