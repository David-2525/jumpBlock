#include "mapa3.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "pausa.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* mapa3::createScene() {
    return mapa3::create();
}

bool mapa3::init() {
    if (!Scene::init()) {
        return false;
    }

    // Inicializar variables
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
    fondo = Sprite::create("background3.png");
    fondo->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height / 2 + origen.y));
    fondo->setScale(2.0f);
    this->addChild(fondo, -1);


    //crear portales 
    portal1 = Sprite::create("portal.png");
    portal1->setPosition(Vec2(tamanoVisible.width * 3.5f + origen.x,
        tamanoVisible.height * 0.4f + origen.y));
    portal1->setScale(1.3f);
    this->addChild(portal1, 1);

    portal2 = Sprite::create("portal.png");
    portal2->setPosition(Vec2(tamanoVisible.width * 8.0f + origen.x,
        tamanoVisible.height * 0.4f + origen.y));
    portal2->setScale(1.3f);
    this->addChild(portal2, 1);

    // Crear una copia del fondo para efecto continuo
    fondoCopia = Sprite::create("background3.png");
    fondoCopia->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height / 2 + origen.y));
    fondoCopia->setScale(2.0f);
    this->addChild(fondoCopia, -1);

    // Crear la plataforma principal
    plataformaPrincipal = Sprite::create("platform_main.png");
    plataformaPrincipal->setScale((tamanoVisible.width * 50) / plataformaPrincipal->getContentSize().width, 1.0f);
    plataformaPrincipal->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height * 0.1f + origen.y));
    this->addChild(plataformaPrincipal, 0);
    plataformas.pushBack(plataformaPrincipal);

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

    // Crear plataformas adicionales
    std::vector<Vec2> posicionesPlataformas = {
        Vec2(tamanoVisible.width * 1.7f + origen.x, tamanoVisible.height * 0.27f + origen.y),
        Vec2(tamanoVisible.width * 2.1f + origen.x, tamanoVisible.height * 0.35f + origen.y),

    };

    int index = 0;
    for (const auto& posicion : posicionesPlataformas) {
        auto plataforma = Sprite::create("platform.png");

        // Ajustar escala según la posición
        if (index == 0) {
            plataforma->setScale(1.2f, 0.6f);
        }
        else if (index == 2) {
            plataforma->setScale(0.8f, 0.6f);
        }
        else {
            plataforma->setScale(0.9f, 0.6f);
        }

        plataforma->setPosition(posicion);
        plataforma->setTag(1);
        this->addChild(plataforma, 0);
        plataformas.pushBack(plataforma);
        index++;
    }

    // Crear el jugador
    jugador = Sprite::create("player2.png");
    jugador->setPosition(Vec2(
        tamanoVisible.width * 0.2f + origen.x,
        plataformaPrincipal->getPositionY() + plataformaPrincipal->getContentSize().height / 2
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

    // Crear obstáculos
    crearMapaConObstaculos();

    // Crear la meta
    meta = Sprite::create("goal.png");
    meta->setPosition(Vec2(tamanoVisible.width * 14.0f + origen.x, tamanoVisible.height * 0.35f + origen.y));
    meta->setScale(0.2f);
    this->addChild(meta, 1);

    // Configurar movimiento continuo
    this->schedule([=](float delta) {
        float velocidadMovimiento = 300.0f;

        // Movimiento del fondo
        fondo->setPositionX(fondo->getPositionX() - velocidadMovimiento * delta);
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

        // Movimiento de la meta
        meta->setPositionX(meta->getPositionX() - velocidadMovimiento * delta);
        }, "MovimientoContinuo");

    // Configurar entrada táctil
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->onTouchBegan = CC_CALLBACK_2(mapa3::alTocarPantalla, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);

    this->scheduleUpdate();

    // Iniciar música de fondo
    idMusicaFondo = AudioEngine::play2d("background_music3.mp3", true);

    return true;
}

void mapa3::crearMapaConObstaculos() {
    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    std::vector<Vec2> posicionesObstaculos = {
        Vec2(tamanoVisible.width * 1.0f + origen.x, plataformaPrincipal->getPositionY() + 400),
        Vec2(tamanoVisible.width * 1.19f + origen.x, plataformaPrincipal->getPositionY() + 400),
        Vec2(tamanoVisible.width * 1.5f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.7f + origen.x, plataformaPrincipal->getPositionY() + 150),

       
    };

    for (const auto& posicion : posicionesObstaculos) {
        auto obstaculo = Sprite::create("obstacle1.png");
        obstaculo->setPosition(posicion);
        obstaculo->setScale(0.2f, 0.2f);
        this->addChild(obstaculo);
        obstaculos.pushBack(obstaculo);
    }

    

    std::vector<Vec2> posicionesTrampasSalto = {
       Vec2(tamanoVisible.width * 1.0f + origen.x, plataformaPrincipal->getPositionY() + 65),
       Vec2(tamanoVisible.width * 1.19f + origen.x, plataformaPrincipal->getPositionY() + 65),
    
    };

    for (const auto& posicion : posicionesTrampasSalto) {
        auto trampaSalto = Sprite::create("obstacle_jump.png");
        trampaSalto->setPosition(posicion);
        trampaSalto->setScale(0.3f, 0.3f);
        trampaSalto->setTag(2);
        this->addChild(trampaSalto);
        obstaculos.pushBack(trampaSalto);
    }
}

bool mapa3::alTocarPantalla(Touch* toque, Event* evento) {
    if (esModoNave) {
        velocidadVertical = FUERZA_NAVE;
    }
    else if (estadoJugador == EstadoJugador::EnPlataforma) {
        estadoJugador = EstadoJugador::Saltando;
        velocidadVertical = FUERZA_SALTO;
        cocos2d::AudioEngine::play2d("jump_sound_effect.mp3");
    }
    return true;
}

void mapa3::update(float delta) {
    // Actualizar temporizador
    tiempoRestante -= delta;
    labelTiempo->setString("Tiempo: " + std::to_string((int)tiempoRestante));

    if (tiempoRestante <= 0) {
        detenerMusica();
        cocos2d::AudioEngine::play2d("death_sound.mp3");
        auto gameOverScene = GameOverScene::createScene(3);
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
    }

    // Verificar victoria
    if (!metaAlcanzada && jugador->getBoundingBox().intersectsRect(meta->getBoundingBox())) {
        metaAlcanzada = true;
        AudioEngine::play2d("victory.mp3");

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
        portalActivo = false;
    }
    else if (!portalActivo) {
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

    auto tamanoVisible = Director::getInstance()->getVisibleSize();

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
        // Actualizar física del jugador
        if (estadoJugador != EstadoJugador::EnPlataforma) {
            // Aplicar gravedad
            velocidadVertical += GRAVEDAD * delta;

            // Actualizar posición
            float nuevaPosY = jugador->getPositionY() + velocidadVertical * delta;
            jugador->setPositionY(nuevaPosY);

            if (velocidadVertical < 0) {
                estadoJugador = EstadoJugador::Cayendo;
            }
        }
        jugador->setRotation(0);
    }

    // Verificar colisiones
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
            velocidadVertical <= 0 && !esModoNave) {

            jugadorSobrePlataforma = true;
            float nuevaAltura = plataforma->getPositionY() +
                (plataforma->getContentSize().height * plataforma->getScaleY() * 0.5f);

            jugador->setPositionY(nuevaAltura);
            estadoJugador = EstadoJugador::EnPlataforma;
            velocidadVertical = 0;
            break;
        }
    }

    if (!jugadorSobrePlataforma && estadoJugador == EstadoJugador::EnPlataforma) {
        estadoJugador = EstadoJugador::Cayendo;
    }

    // Verificar colisiones con obstáculos
    for (auto& obstaculo : obstaculos) {
        auto obstaculoBoundingBox = obstaculo->getBoundingBox();
        if (jugadorBoundingBox.intersectsRect(obstaculoBoundingBox)) {
            if (obstaculo->getTag() == 2) { // Obstáculo que hace saltar
                if ((estadoJugador == EstadoJugador::EnPlataforma ||
                    estadoJugador == EstadoJugador::Cayendo) && !esModoNave) {
                    estadoJugador = EstadoJugador::Saltando;
                    velocidadVertical = FUERZA_SALTO * 2.0f;
                }
            }
            else {
                detenerMusica();
                cocos2d::AudioEngine::play2d("death_sound.mp3");
                auto gameOverScene = GameOverScene::createScene(3);
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
            }
        }
    }
}

void mapa3::detenerMusica() {
    if (idMusicaFondo != AudioEngine::INVALID_AUDIO_ID) {
        AudioEngine::stop(idMusicaFondo);
    }
}

void mapa3::menuCerrarCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}