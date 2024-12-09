#include "mapa2.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "pausa.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* mapa2::createScene() {
    return mapa2::create();
}

bool mapa2::init() {
    if (!Scene::init()) {
        return false;
    }

    estadoJugador = EstadoJugador::EnPlataforma;
    velocidadVertical = 0.0f;
    estaSaltando = false;
    estaEnPlataforma = false;
    tiempoRestante = 60.0f;
    metaAlcanzada = false;
    esModoNave = false;
    portalActivo = false;           
    ultimoPortalTocado = nullptr;

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Crear el fondo
    fondo = Sprite::create("background2.png");  
    fondo->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height / 2 + origen.y));
    fondo->setScale(2.5f);
    this->addChild(fondo, -1);

    // Crear una copia del fondo para efecto continuo
    fondoCopia = Sprite::create("background2.png"); 
    fondoCopia->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height / 2 + origen.y));
    fondoCopia->setScale(2.5f);
    this->addChild(fondoCopia, -1);

    // Crear la plataforma principal
    plataformaPrincipal = Sprite::create("platform_main.png");  
    plataformaPrincipal->setScale((tamanoVisible.width * 60) / plataformaPrincipal->getContentSize().width, 1.0f);
    plataformaPrincipal->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height * 0.1f + origen.y));
    this->addChild(plataformaPrincipal, 0);
    plataformas.pushBack(plataformaPrincipal);

    // Crear plataformas adicionales
    std::vector<Vec2> posicionesPlataformas = {
    Vec2(tamanoVisible.width * 1.7f + origen.x, tamanoVisible.height * 0.27f + origen.y),
    Vec2(tamanoVisible.width * 2.8f + origen.x, tamanoVisible.height * 0.5f + origen.y),
    Vec2(tamanoVisible.width * 3.5f + origen.x, tamanoVisible.height * 0.2f + origen.y),
    Vec2(tamanoVisible.width * 7.0f + origen.x, tamanoVisible.height * 0.2f + origen.y),
    Vec2(tamanoVisible.width * 7.0f + origen.x, tamanoVisible.height * 0.72f + origen.y),
    Vec2(tamanoVisible.width * 7.8f + origen.x, plataformaPrincipal->getPositionY() + 110),
    Vec2(tamanoVisible.width * 8.6f + origen.x, plataformaPrincipal->getPositionY() + 400),
    Vec2(tamanoVisible.width * 9.0f + origen.x, plataformaPrincipal->getPositionY() + 300),
    Vec2(tamanoVisible.width * 9.45f + origen.x, plataformaPrincipal->getPositionY() + 200),
    Vec2(tamanoVisible.width * 11.6f + origen.x, plataformaPrincipal->getPositionY() + 80),
    Vec2(tamanoVisible.width * 9.95f + origen.x, plataformaPrincipal->getPositionY() + 100),
    Vec2(tamanoVisible.width * 11.85f + origen.x, plataformaPrincipal->getPositionY() + 100),
    Vec2(tamanoVisible.width * 12.5f + origen.x, plataformaPrincipal->getPositionY() + 300),

    };

    for (size_t i = 0; i < posicionesPlataformas.size(); ++i) {
        auto plataforma = Sprite::create("platform.png"); 

        if (i == 2) {
            plataforma->setScale(0.25f, 1.6f);
        }
        else if (i ==3) {
            plataforma->setScale(0.25f, 1.7f);
        }
        else if (i ==4) {
            plataforma->setScale(0.25f, 6.0f);
        }
        else if (i == 9) {
            plataforma->setScale(0.3f, 1.6f);
        }
        else if (i == 11) {
            plataforma->setScale(0.3f, 2.3f);
        }
        else {
            plataforma->setScale(1.0f, 0.6f);
        }

        plataforma->setPosition(posicionesPlataformas[i]);
        plataforma->setTag(1);
        this->addChild(plataforma, 0);
        plataformas.pushBack(plataforma);
    }

    // Crear el jugador
    jugador = Sprite::create("player2.png");  
    jugador->setPosition(Vec2(
        tamanoVisible.width * 0.2f + origen.x,
        plataformaPrincipal->getPositionY() + plataformaPrincipal->getContentSize().height / 2+8.0f
    ));
    jugador->setScale(0.35f);
    this->addChild(jugador, 10);

    // Crear temporizador
    labelTiempo = Label::createWithSystemFont("Tiempo: 60", "Arial", 24);
    labelTiempo->setPosition(Vec2(tamanoVisible.width * 0.9f + origen.x, tamanoVisible.height * 0.95f + origen.y));
    this->addChild(labelTiempo, 10);

    // Crear botón de pausa
    auto botonPausa = ui::Button::create("boton_pausa.png");  
    botonPausa->setPosition(Vec2(
        tamanoVisible.width * 0.05f + origen.x,
        tamanoVisible.height * 0.95f + origen.y
    ));
    botonPausa->setScale(0.2f);
    botonPausa->addClickEventListener([this](Ref* sender) {
        auto escenaPausa = pausa::createScene(this);
        Director::getInstance()->pushScene(escenaPausa);
        this->pause();
        AudioEngine::pauseAll();
        });
    this->addChild(botonPausa, 10);

    // Crear los portales
    portal1 = Sprite::create("portal.png");
    portal1->setPosition(Vec2(tamanoVisible.width * 3.5f + origen.x,
        tamanoVisible.height * 0.4f + origen.y));
    portal1->setScale(1.3f);
    this->addChild(portal1, 1);

    portal2 = Sprite::create("portal.png");
    portal2->setPosition(Vec2(tamanoVisible.width * 7.0f + origen.x,
        tamanoVisible.height * 0.4f + origen.y));
    portal2->setScale(1.3f);
    this->addChild(portal2, 1);


    // Crear obstáculos
    crearMapaConObstaculos();

    // Crear la meta
    meta = Sprite::create("goal.png");  
    meta->setPosition(Vec2(tamanoVisible.width * 14.0f + origen.x, tamanoVisible.height * 0.35f + origen.y));
    meta->setScale(0.2f);
    this->addChild(meta, 1);

    // Configurar movimiento continuo
    this->schedule([=](float delta) {
        float velocidadMovimiento = 400.0f;

        // Movimiento del fondo
        fondo->setPositionX(fondo->getPositionX() - velocidadMovimiento * delta);//delata es para asegurar que la fisica sea la misma, el tiempo 
        fondoCopia->setPositionX(fondoCopia->getPositionX() - velocidadMovimiento * delta);

        if (fondo->getPositionX() <= -tamanoVisible.width / 2) {
            fondo->setPositionX(fondoCopia->getPositionX() + tamanoVisible.width);
        }
        if (fondoCopia->getPositionX() <= -tamanoVisible.width / 2) {
            fondoCopia->setPositionX(fondo->getPositionX() + tamanoVisible.width);
        }

        // Movimiento de plataformas y obstáculos
        for (auto& plataforma : plataformas) {
            plataforma->setPositionX(plataforma->getPositionX() - velocidadMovimiento * delta);
        }

        for (auto& obstaculo : obstaculos) {
            obstaculo->setPositionX(obstaculo->getPositionX() - velocidadMovimiento * delta);
        }

        // Mover portales
        portal1->setPositionX(portal1->getPositionX() - velocidadMovimiento * delta);
        portal2->setPositionX(portal2->getPositionX() - velocidadMovimiento * delta);

        // Movimiento de la meta
        meta->setPositionX(meta->getPositionX() - velocidadMovimiento * delta);
        }, "MovimientoContinuo");

    // Configurar entrada táctil
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->onTouchBegan = CC_CALLBACK_2(mapa2::alTocarPantalla, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);

    this->scheduleUpdate();

    // Iniciar música de fondo
    AudioEngine::play2d("background_music2.mp3", true);

    return true;
}

void mapa2::crearMapaConObstaculos() {
    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    std::vector<Vec2> posicionesObstaculos = {
        Vec2(tamanoVisible.width * 0.9f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.7f + origen.x, plataformaPrincipal->getPositionY() + 170),
        Vec2(tamanoVisible.width * 1.45f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.3f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.35f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.4f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.8f + origen.x, plataformaPrincipal->getPositionY() + 550),
        Vec2(tamanoVisible.width * 3.5f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.5f + origen.x, plataformaPrincipal->getPositionY() + 105),

        Vec2(tamanoVisible.width * 4.0f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 4.2f + origen.x, tamanoVisible.height * 0.7f + origen.y),
        Vec2(tamanoVisible.width * 4.4f + origen.x, tamanoVisible.height * 0.3f + origen.y),

        Vec2(tamanoVisible.width * 5.0f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 5.2f + origen.x, tamanoVisible.height * 0.6f + origen.y),
        Vec2(tamanoVisible.width * 5.4f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 5.6f + origen.x, tamanoVisible.height * 0.7f + origen.y),

        Vec2(tamanoVisible.width * 6.0f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 6.2f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 6.4f + origen.x, tamanoVisible.height * 0.6f + origen.y),
        Vec2(tamanoVisible.width * 6.6f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 7.5f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.55f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.8f + origen.x, plataformaPrincipal->getPositionY() + 150),
        Vec2(tamanoVisible.width * 8.15f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 8.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 8.7f + origen.x, plataformaPrincipal->getPositionY() + 450),
        Vec2(tamanoVisible.width * 9.1f + origen.x, plataformaPrincipal->getPositionY() + 350),
        Vec2(tamanoVisible.width * 9.55f + origen.x, plataformaPrincipal->getPositionY() + 250),
        Vec2(tamanoVisible.width * 9.95f + origen.x, plataformaPrincipal->getPositionY() + 150),
        Vec2(tamanoVisible.width * 10.66f + origen.x, plataformaPrincipal->getPositionY() + 400),
        Vec2(tamanoVisible.width * 10.95f + origen.x, plataformaPrincipal->getPositionY() + 400),
        Vec2(tamanoVisible.width * 11.52f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 11.73f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 12.6f + origen.x, plataformaPrincipal->getPositionY() + 350),
        Vec2(tamanoVisible.width * 13.1f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 13.15f + origen.x, plataformaPrincipal->getPositionY() + 65),

        
    };

    for (const auto& posicion : posicionesObstaculos) {
        auto obstaculo = Sprite::create("obstacle1.png");  
        obstaculo->setPosition(posicion);
        obstaculo->setScale(0.25f, 0.25f);
        this->addChild(obstaculo);
        obstaculos.pushBack(obstaculo);
    }

    std::vector<Vec2> posicionesTrampasSalto = {
        Vec2(tamanoVisible.width * 2.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.8f + origen.x, plataformaPrincipal->getPositionY() + 330),
        Vec2(tamanoVisible.width * 8.0f + origen.x, plataformaPrincipal->getPositionY() + 150),
        Vec2(tamanoVisible.width * 10.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 10.9f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 11.85f + origen.x, plataformaPrincipal->getPositionY() + 230),

    };

    for (const auto& posicion : posicionesTrampasSalto) {
        auto trampaSalto = Sprite::create("obstacle_jump.png");  
        trampaSalto->setPosition(posicion);
        trampaSalto->setScale(0.35f, 0.35f);
        trampaSalto->setTag(2);
        this->addChild(trampaSalto);
        obstaculos.pushBack(trampaSalto);
    }
}

bool mapa2::alTocarPantalla(Touch* toque, Event* evento) {
    if (esModoNave) {
        // En modo nave, dar impulso hacia arriba
        velocidadVertical = FUERZA_NAVE;
    }
    else if (estadoJugador == EstadoJugador::EnPlataforma) {
        estadoJugador = EstadoJugador::Saltando;
        velocidadVertical = FUERZA_SALTO;
    }
    return true;
}

void mapa2::cambiarModoJugador() {
    if (!esModoNave) {
        // Transformar en nave
        esModoNave = true;
        jugador->setTexture("nave.png");
        jugador->setScale(0.08f);
        estadoJugador = EstadoJugador::ModoNave;
    }
    else {
        // Volver a modo normal
        esModoNave = false;
        jugador->setTexture("player2.png");
        jugador->setScale(0.35f);
        estadoJugador = EstadoJugador::Cayendo;
    }
}


void mapa2::update(float delta) {
    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    // Actualizar temporizador
    tiempoRestante -= delta;
    labelTiempo->setString("Tiempo: " + std::to_string((int)tiempoRestante));

    if (tiempoRestante <= 0) {
        AudioEngine::stopAll();
        auto gameOverScene = GameOverScene::createScene(2);
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
    }

    // Verificar victoria
    if (!metaAlcanzada && jugador->getBoundingBox().intersectsRect(meta->getBoundingBox())) {
        metaAlcanzada = true;
        AudioEngine::stopAll();

        this->scheduleOnce([](float) {
            auto menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
            }, 1.0f, "transitionToMenu");
        return;
    }

    // lógica de portales
    auto jugadorBoundingBox = jugador->getBoundingBox();
    bool colisionPortal1 = jugadorBoundingBox.intersectsRect(portal1->getBoundingBox());
    bool colisionPortal2 = jugadorBoundingBox.intersectsRect(portal2->getBoundingBox());

    if (!colisionPortal1 && !colisionPortal2) {
        // Si no está en ningún portal, permitir nueva detección
        portalActivo = false;
    }
    else if (!portalActivo) {
        // Si no hay portal activo y está en alguno, activar el cambio
        if (colisionPortal1 && ultimoPortalTocado != portal1) {
            ultimoPortalTocado = portal1;
            portalActivo = true;
            esModoNave = !esModoNave;
            if (esModoNave) {
                jugador->setTexture("nave.png");
                jugador->setScale(0.15f);
                estadoJugador = EstadoJugador::ModoNave;
            }
            else {
                jugador->setTexture("player2.png");
                jugador->setScale(0.35f);
                estadoJugador = EstadoJugador::Cayendo;
            }
        }
        else if (colisionPortal2 && ultimoPortalTocado != portal2) {
            ultimoPortalTocado = portal2;
            portalActivo = true;
            esModoNave = !esModoNave;
            if (esModoNave) {
                jugador->setTexture("nave.png");
                jugador->setScale(0.15f);
                estadoJugador = EstadoJugador::ModoNave;
            }
            else {
                jugador->setTexture("player2.png");
                jugador->setScale(0.35f);
                estadoJugador = EstadoJugador::Cayendo;
            }
        }
    }

    // Actualizar física según el modo
    if (esModoNave) {
        velocidadVertical += GRAVEDAD_NAVE * delta;
        float nuevaPosY = jugador->getPositionY() + velocidadVertical * delta;

        // Limitar el movimiento vertical
        if (nuevaPosY > tamanoVisible.height * 0.9f) {
            nuevaPosY = tamanoVisible.height * 0.9f;
            velocidadVertical = 0;
        }
        if (nuevaPosY < tamanoVisible.height * 0.1f) {
            nuevaPosY = tamanoVisible.height * 0.1f;
            velocidadVertical = 0;
        }

        jugador->setPositionY(nuevaPosY);

        // Rotar la nave según la velocidad
        float rotacion = velocidadVertical * 0.1f;
        rotacion = std::max(-45.0f, std::min(45.0f, rotacion));
        jugador->setRotation(-rotacion);
    }
    else {
        // Física normal del personaje
        if (estadoJugador != EstadoJugador::EnPlataforma) {
            velocidadVertical += GRAVEDAD * delta;
            float nuevaPosY = jugador->getPositionY() + velocidadVertical * delta;
            jugador->setPositionY(nuevaPosY);

            if (velocidadVertical < 0) {
                estadoJugador = EstadoJugador::Cayendo;
            }
        }
        jugador->setRotation(0);
    }

    if (!esModoNave) {
        auto jugadorBoundingBox = jugador->getBoundingBox();
        bool jugadorSobrePlataforma = false;

        // Verificar colisiones con plataformas 
        for (auto& plataforma : plataformas) {
            auto plataformaBoundingBox = plataforma->getBoundingBox();
            float alturaPiePlataforma = plataformaBoundingBox.size.height * 0.2f;

            Rect areaColisionPlataforma(
                plataformaBoundingBox.origin.x,
                plataformaBoundingBox.origin.y + plataformaBoundingBox.size.height - alturaPiePlataforma,
                plataformaBoundingBox.size.width,
                alturaPiePlataforma
            );

            if (jugadorBoundingBox.intersectsRect(areaColisionPlataforma) &&
                velocidadVertical <= 0) {

                jugadorSobrePlataforma = true;
                float nuevaAltura = plataforma->getPositionY() +
                    (plataforma->getContentSize().height * plataforma->getScaleY() * 0.5f) + 8.0f;

                jugador->setPositionY(nuevaAltura);
                estadoJugador = EstadoJugador::EnPlataforma;
                velocidadVertical = 0;
                break;
            }
        }

        if (!jugadorSobrePlataforma && estadoJugador == EstadoJugador::EnPlataforma) {
            estadoJugador = EstadoJugador::Cayendo;
        }
    }

    // Verificar colisiones con obstáculos
    for (auto& obstaculo : obstaculos) {
        auto obstaculoBoundingBox = obstaculo->getBoundingBox();
        Rect areaColisionObstaculo(
            obstaculoBoundingBox.origin.x + (obstaculoBoundingBox.size.width * 0.2f),
            obstaculoBoundingBox.origin.y + (obstaculoBoundingBox.size.height * 0.2f),
            obstaculoBoundingBox.size.width * 0.6f,
            obstaculoBoundingBox.size.height * 0.6f
        );

        if (jugadorBoundingBox.intersectsRect(areaColisionObstaculo)) {
            if (obstaculo->getTag() == 2) { // Trampa de salto
                if (estadoJugador == EstadoJugador::EnPlataforma || estadoJugador == EstadoJugador::Cayendo) {
                    estadoJugador = EstadoJugador::Saltando;
                    velocidadVertical = FUERZA_SALTO * 2.0f;
                }
            }
            else { // Obstáculo normal
                AudioEngine::stopAll();
                AudioEngine::play2d("death_sound.mp3", false);
                auto gameOverScene = GameOverScene::createScene(2);
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
            }
        }
    }
}
void mapa2::menuCerrarCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}