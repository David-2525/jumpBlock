#ifndef __MAPA1_SCENE_H__
#define __MAPA1_SCENE_H__

#include "cocos2d.h"

class mapa1 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Crear el mapa con obst�culos y meta
    void crearMapaConObstaculos();

    // Callback para manejar la interacci�n del jugador
    bool alTocarPantalla(cocos2d::Touch* toque, cocos2d::Event* evento);

    // Funci�n de actualizaci�n de la escena
    void update(float delta) override;

    // M�todo para cerrar el juego
    void menuCerrarCallback(cocos2d::Ref* pSender);

    // Funci�n para aplicar deslizamiento en plataformas secundarias
    void aplicarDeslizamiento(cocos2d::Sprite* plataforma);

    // Crea la macro create()
    CREATE_FUNC(mapa1);

private:
    float plataformaAnteriorX = 0.0f;
    cocos2d::Sprite* jugador;                      // Sprite del jugador
    cocos2d::Sprite* plataformaPrincipal;          // Plataforma base del nivel
    cocos2d::Sprite* fondo;                        // Fondo principal
    cocos2d::Sprite* fondoCopia;                   // Copia del fondo para desplazamiento continuo
    cocos2d::Vector<cocos2d::Sprite*> obstaculos;  // Vector de obst�culos
    cocos2d::Vector<cocos2d::Sprite*> plataformas; // Vector de plataformas adicionales
    bool estaSaltando = false;                     // Estado del salto del jugador
    bool estaEnPlataforma = false;                 // Estado de si est� sobre una plataforma
    float tiempoTranscurrido = 0.0f;               // Tiempo transcurrido para controlar el desplazamiento del mapa
};

#endif // __MAPA1_SCENE_H__
