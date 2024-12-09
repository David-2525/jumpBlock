#ifndef __MAPA3_SCENE_H__
#define __MAPA3_SCENE_H__

#include "cocos2d.h"

class mapa3 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void crearMapaConObstaculos();
    bool alTocarPantalla(cocos2d::Touch* toque, cocos2d::Event* evento);
    void update(float delta) override;
    void menuCerrarCallback(cocos2d::Ref* pSender);
    void cambiarModoJugador();
    void detenerMusica();

    CREATE_FUNC(mapa3);

private:
    enum class EstadoJugador {
        EnPlataforma,
        Saltando,
        Cayendo,
        ModoNave
    };

    // Constantes f�sicas
    const float GRAVEDAD = -1200.0f;
    const float FUERZA_SALTO = 550.0f;
    const float VELOCIDAD_MOVIMIENTO = 450.0f;
    const float FUERZA_NAVE = 200.0f;
    const float GRAVEDAD_NAVE = -300.0f;

    // Variables de estado del jugador
    float velocidadVertical;
    bool estaSaltando;
    bool estaEnPlataforma;
    bool metaAlcanzada;
    bool esModoNave;
    EstadoJugador estadoJugador;

    // Control de portales
    bool portalActivo;
    cocos2d::Sprite* ultimoPortalTocado;

    // Tiempo y m�sica
    float tiempoRestante;
    float tiempoTranscurrido;
    cocos2d::Label* labelTiempo;
    int idMusicaFondo;

    // Sprites del juego
    cocos2d::Sprite* jugador;
    cocos2d::Sprite* plataformaPrincipal;
    cocos2d::Sprite* fondo;
    cocos2d::Sprite* fondoCopia;
    cocos2d::Sprite* meta;
    cocos2d::Sprite* portal1;
    cocos2d::Sprite* portal2;

    // Contenedores de elementos del juego
    cocos2d::Vector<cocos2d::Sprite*> obstaculos;
    cocos2d::Vector<cocos2d::Sprite*> plataformas;
};

#endif // __MAPA3_SCENE_H__
