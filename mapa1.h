#ifndef __MAPA1_SCENE_H__
#define __MAPA1_SCENE_H__

#include "cocos2d.h"

class mapa1 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void crearMapaConObstaculos();
    bool alTocarPantalla(cocos2d::Touch* toque, cocos2d::Event* evento);
    void update(float delta) override;
    void menuCerrarCallback(cocos2d::Ref* pSender);
    void aplicarDeslizamiento(cocos2d::Sprite* plataforma);
    void detenerMusica(); 

    CREATE_FUNC(mapa1);

private:
    enum class EstadoJugador {
        EnPlataforma,
        Saltando,
        Cayendo
    };

    // Variables físicas del salto
    const float GRAVEDAD = -1200.0f;
    const float FUERZA_SALTO = 550.0f;

    // Variables de movimiento y tiempo
    float velocidadVertical = 0.0f;
    float velocidadMovimiento = 300.0f;
    float tiempoRestante;
    float tiempoTranscurrido = 0.0f;

    // Estado del jugador y elementos del mapa
    EstadoJugador estadoJugador = EstadoJugador::EnPlataforma;
    cocos2d::Sprite* jugador;
    cocos2d::Sprite* plataformaPrincipal;
    cocos2d::Sprite* fondo;
    cocos2d::Sprite* fondoCopia;
    cocos2d::Sprite* meta;
    cocos2d::Vector<cocos2d::Sprite*> obstaculos;
    cocos2d::Vector<cocos2d::Sprite*> plataformas;
    cocos2d::Label* labelTiempo;

    int idMusicaFondo; 

    bool estaSaltando = false;
    bool estaEnPlataforma = false;
    bool metaAlcanzada = false;
};

#endif // __MAPA1_SCENE_H__

