#include "mapa1.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "pausa.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* mapa1::createScene() {
    return mapa1::create();
}

bool mapa1::init() {
    if (!Scene::init()) {
        return false;
    }

    estadoJugador = EstadoJugador::EnPlataforma;
    velocidadVertical = 0.0f;
    estaSaltando = false;
    estaEnPlataforma = false;
    tiempoRestante = 60.0f;

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Crear el fondo
    fondo = Sprite::create("background.png");
    fondo->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height / 2 + origen.y));
    fondo->setScale(2.0f);
    this->addChild(fondo, -1);

    // Crear una copia del fondo para efecto continuo
    fondoCopia = Sprite::create("background.png");
    fondoCopia->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height / 10 + origen.y));
    fondoCopia->setScale(2.0f);
    this->addChild(fondoCopia, -1);

    // Crear la plataforma principal
    plataformaPrincipal = Sprite::create("platform_main.png");
    plataformaPrincipal->setScale((tamanoVisible.width * 50) / plataformaPrincipal->getContentSize().width, 1.0f);
    plataformaPrincipal->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height * 0.1f + origen.y));
    this->addChild(plataformaPrincipal, 0);
    plataformas.pushBack(plataformaPrincipal);

    // Crear plataformas adicionales
    std::vector<Vec2> posicionesPlataformas = {
        Vec2(tamanoVisible.width * 1.6f + origen.x, tamanoVisible.height * 0.27f + origen.y),
        Vec2(tamanoVisible.width * 2.1f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 2.5f + origen.x, tamanoVisible.height * 0.15f + origen.y),
        Vec2(tamanoVisible.width * 2.74f + origen.x, tamanoVisible.height * 0.15f + origen.y),
        Vec2(tamanoVisible.width * 2.94f + origen.x, tamanoVisible.height * 0.15f + origen.y),
        Vec2(tamanoVisible.width * 3.2f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 3.6f + origen.x, tamanoVisible.height * 0.55f + origen.y),
        Vec2(tamanoVisible.width * 3.95f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 3.95f + origen.x, tamanoVisible.height * 0.9f + origen.y),
        Vec2(tamanoVisible.width * 4.2f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 4.35f + origen.x, tamanoVisible.height * 0.2f + origen.y),
        Vec2(tamanoVisible.width * 5.1f + origen.x, tamanoVisible.height * 0.25f + origen.y),
        Vec2(tamanoVisible.width * 5.25f + origen.x, tamanoVisible.height * 0.2f + origen.y),
        Vec2(tamanoVisible.width * 5.25f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 5.53f + origen.x, tamanoVisible.height * 0.55f + origen.y),
        Vec2(tamanoVisible.width * 5.4f + origen.x, tamanoVisible.height * 0.33f + origen.y),
        Vec2(tamanoVisible.width * 6.3f + origen.x, tamanoVisible.height * 0.45f + origen.y),
        Vec2(tamanoVisible.width * 6.6f + origen.x, tamanoVisible.height * 0.6f + origen.y),
        Vec2(tamanoVisible.width * 6.9f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 7.15f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 7.4f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 7.9f + origen.x, tamanoVisible.height * 0.57f + origen.y),
        Vec2(tamanoVisible.width * 8.3f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 8.6f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 9.0f + origen.x, tamanoVisible.height * 0.49f + origen.y),
        Vec2(tamanoVisible.width * 9.45f + origen.x, tamanoVisible.height * 0.63f + origen.y),
        Vec2(tamanoVisible.width * 9.9f + origen.x, tamanoVisible.height * 0.55f + origen.y),
        Vec2(tamanoVisible.width * 10.4f + origen.x, tamanoVisible.height * 0.45f + origen.y),
        Vec2(tamanoVisible.width * 10.8f + origen.x, tamanoVisible.height * 0.5f + origen.y),
        Vec2(tamanoVisible.width * 11.3f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 13.4f + origen.x, tamanoVisible.height * 0.3f + origen.y),
        Vec2(tamanoVisible.width * 13.8f + origen.x, tamanoVisible.height * 0.25f + origen.y),

    };

    int index = 0;
    for (const auto& posicion : posicionesPlataformas) {
        auto plataforma = Sprite::create("platform.png");

        if (index == 0) {
            plataforma->setScale(1.2f, 0.6f);
        }
        else if (index == 2) {
            plataforma->setScale(0.35f, 1.3f);
        }
        else if (index == 3) {
            plataforma->setScale(0.25f, 1.6f);
        }
        else if (index == 4) {
            plataforma->setScale(0.25f, 1.9f);
        }
        else if (index == 5) {
            plataforma->setScale(0.6f, 0.6f);
        }
        else if (index == 6) {
            plataforma->setScale(0.6f, 0.6f);
        }
        else if (index == 9) {
            plataforma->setScale(0.3f, 0.6f);
        }
        else if (index == 10) {
            plataforma->setScale(0.3f, 0.6f);
        }
        else if (index == 11) {
            plataforma->setScale(0.5f, 0.6f);
        }
        else if (index == 12) {
            plataforma->setScale(0.3f, 0.6f);
        }
        else if (index == 13) {
            plataforma->setScale(0.4f, 0.6f);
        }
        else if (index == 14) {
            plataforma->setScale(1.3f, 0.6f);
        }
        else if (index == 15) {
            plataforma->setScale(0.4f, 0.6f);
        }
        else if (index == 16) {
            plataforma->setScale(0.6f, 0.6f);
        }
        else if (index == 17) {
            plataforma->setScale(0.5f, 0.6f);
        }
        else if (index == 18) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 19) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 20) {
            plataforma->setScale(0.8f, 0.6f);
        }
        else if (index == 21) {
            plataforma->setScale(1.2f, 0.6f);
        }
        else if (index == 22) {
            plataforma->setScale(0.4f, 0.6f);
        }
        else if (index == 23) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 26) {
            plataforma->setScale(0.9f, 0.6f);
        }
        else if (index == 27) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 30) {
            plataforma->setScale(0.8f, 0.6f);
        }
        else if (index == 31) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 32) {
            plataforma->setScale(0.6f, 0.6f);
        }
        else if (index == 33) {
            plataforma->setScale(0.9f, 0.6f);
        }
        else if (index == 34) {
            plataforma->setScale(0.7f, 0.6f);
        }
        else if (index == 35) {
            plataforma->setScale(0.8f, 0.6f);
        }
        else {
            plataforma->setScale(1.1f, 0.6f);
        }

        plataforma->setPosition(posicion);
        plataforma->setTag(1);
        this->addChild(plataforma, 0);
        plataformas.pushBack(plataforma);
        index++;
    }

    // Crear el jugador
    jugador = Sprite::create("player.png");
    jugador->setPosition(Vec2(
        tamanoVisible.width * 0.2f + origen.x,
        plataformaPrincipal->getPositionY() + plataformaPrincipal->getContentSize().height / 2
    ));
    jugador->setScale(1.0f);
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
    meta->setPosition(Vec2(tamanoVisible.width * 14.0f + origen.x, tamanoVisible.height * 0.25f + origen.y));
    meta->setScale(0.2f);
    this->addChild(meta, 1);

    // la meta para que se mueva con el nivel
    this->schedule([=](float delta) mutable {
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

    // Movimiento continuo
    float velocidad = 300.0f;
    this->schedule([=](float delta) mutable {
        // Movimiento del fondo
        fondo->setPositionX(fondo->getPositionX() - velocidad * delta);
        fondoCopia->setPositionX(fondoCopia->getPositionX() - velocidad * delta);

        if (fondo->getPositionX() <= -tamanoVisible.width / 2) {
            fondo->setPositionX(fondoCopia->getPositionX() + tamanoVisible.width);
        }
        if (fondoCopia->getPositionX() <= -tamanoVisible.width / 2) {
            fondoCopia->setPositionX(fondo->getPositionX() + tamanoVisible.width);
        }

        // Movimiento de plataformas y obstáculos
        for (auto& plataforma : plataformas) {
            plataforma->setPositionX(plataforma->getPositionX() - velocidad * delta);
        }

        for (auto& obstaculo : obstaculos) {
            obstaculo->setPositionX(obstaculo->getPositionX() - velocidad * delta);
        }
        }, "MovimientoContinuo");

    // entrada táctil
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->onTouchBegan = CC_CALLBACK_2(mapa1::alTocarPantalla, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);

    this->scheduleUpdate();

    // Iniciar música de fondo 
    idMusicaFondo = cocos2d::AudioEngine::play2d("background_music.mp3", true);

    return true;
}

void mapa1::crearMapaConObstaculos() {
    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    std::vector<Vec2> posicionesObstaculos = {
        Vec2(tamanoVisible.width * 0.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 0.8f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.0f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.4f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.55f + origen.x, plataformaPrincipal->getPositionY() + 170),
        Vec2(tamanoVisible.width * 1.74f + origen.x, plataformaPrincipal->getPositionY() + 170),
        Vec2(tamanoVisible.width * 2.f + origen.x, plataformaPrincipal->getPositionY() + 250),
        Vec2(tamanoVisible.width * 2.2f + origen.x, plataformaPrincipal->getPositionY() + 250),
        Vec2(tamanoVisible.width * 2.85f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.1f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.25f + origen.x, plataformaPrincipal->getPositionY() + 180),
        Vec2(tamanoVisible.width * 3.65f + origen.x, plataformaPrincipal->getPositionY() + 375),
        Vec2(tamanoVisible.width * 3.98f + origen.x, plataformaPrincipal->getPositionY() + 535),
        Vec2(tamanoVisible.width * 4.45f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 4.5f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 4.55f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 4.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.4f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.45f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.5f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 5.25f + origen.x, plataformaPrincipal->getPositionY() + 120),
        Vec2(tamanoVisible.width * 5.0f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 5.4f + origen.x, plataformaPrincipal->getPositionY() + 210),
        Vec2(tamanoVisible.width * 6.5f + origen.x, plataformaPrincipal->getPositionY() + 260),
        Vec2(tamanoVisible.width * 5.48f + origen.x, plataformaPrincipal->getPositionY() + 373),
        Vec2(tamanoVisible.width * 5.69f + origen.x, plataformaPrincipal->getPositionY() + 373),
        Vec2(tamanoVisible.width * 6.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 6.25f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 6.3f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 6.35f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 6.64f + origen.x, plataformaPrincipal->getPositionY() + 410),
        Vec2(tamanoVisible.width * 6.97f + origen.x, plataformaPrincipal->getPositionY() + 330),
        Vec2(tamanoVisible.width * 7.31f + origen.x, plataformaPrincipal->getPositionY() + 182),
        Vec2(tamanoVisible.width * 7.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.65f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.7f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.9f + origen.x, plataformaPrincipal->getPositionY() + 600),
        Vec2(tamanoVisible.width * 8.27f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 8.32f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 8.37f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 8.9f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 9.0f + origen.x, plataformaPrincipal->getPositionY() + 330),
        Vec2(tamanoVisible.width * 9.3f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 9.35f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 9.45f + origen.x, plataformaPrincipal->getPositionY() + 430),
        Vec2(tamanoVisible.width * 9.8f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 9.85f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 9.9f + origen.x, plataformaPrincipal->getPositionY() + 370),
        Vec2(tamanoVisible.width * 10.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 10.25f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 10.65f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 10.7f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 10.8f + origen.x, plataformaPrincipal->getPositionY() + 550),
        Vec2(tamanoVisible.width * 11.16f + origen.x, plataformaPrincipal->getPositionY() + 260),
        Vec2(tamanoVisible.width * 11.43f + origen.x, plataformaPrincipal->getPositionY() + 260),
        Vec2(tamanoVisible.width * 11.28f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 11.8f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 12.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 12.25f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 12.8f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 13.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 13.4f + origen.x, plataformaPrincipal->getPositionY() + 65),

        Vec2(tamanoVisible.width * 13.4f + origen.x, plataformaPrincipal->getPositionY() + 190),
        Vec2(tamanoVisible.width * 13.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 13.8f + origen.x, plataformaPrincipal->getPositionY() + 155),

    };

    // Posiciones de trampas de salto
    std::vector<Vec2> posicionesTrampasSalto = {
        Vec2(tamanoVisible.width * 2.f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 2.2f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 3.18f + origen.x, plataformaPrincipal->getPositionY() + 180),
        Vec2(tamanoVisible.width * 3.98f + origen.x, plataformaPrincipal->getPositionY() + 255),
        Vec2(tamanoVisible.width * 4.35f + origen.x, plataformaPrincipal->getPositionY() + 120),
        Vec2(tamanoVisible.width * 6.0f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 7.49f + origen.x, plataformaPrincipal->getPositionY() + 180),
        Vec2(tamanoVisible.width * 7.9f + origen.x, plataformaPrincipal->getPositionY() + 380),
        Vec2(tamanoVisible.width * 10.8 + origen.x, plataformaPrincipal->getPositionY() + 330),

    };

    for (const auto& posicion : posicionesObstaculos) {
        auto obstaculo = Sprite::create("obstacle1.png");
        obstaculo->setPosition(posicion);
        obstaculo->setScale(0.2f, 0.2f);
        this->addChild(obstaculo);
        obstaculos.pushBack(obstaculo);
    }
    // Rotar un obstáculo específico 
    if (obstaculos.size() > 7) {
        auto obstaculo = obstaculos.at(6);
        obstaculo->setRotation(180); 
    }
    if (obstaculos.size() > 8) {
        auto obstaculo = obstaculos.at(7);
        obstaculo->setRotation(180);
    }
    if (obstaculos.size() > 13) {
        auto obstaculo = obstaculos.at(12);
        obstaculo->setRotation(180); 
    }
    if (obstaculos.size() > 38) {
        auto obstaculo = obstaculos.at(38);

        obstaculo->setRotation(180); 
    }
    if (obstaculos.size() > 54) {
        auto obstaculo = obstaculos.at(54);

        obstaculo->setRotation(180); 
    }

    // Crear trampas de salto
    for (const auto& posicion : posicionesTrampasSalto) {
        auto trampaSalto = Sprite::create("obstacle_jump.png");
        trampaSalto->setPosition(posicion);
        trampaSalto->setScale(0.3f, 0.3f); 
        trampaSalto->setTag(2);            
        this->addChild(trampaSalto);
        obstaculos.pushBack(trampaSalto);
    }
}

bool mapa1::alTocarPantalla(Touch* toque, Event* evento) {
    if (estadoJugador == EstadoJugador::EnPlataforma) {
        estadoJugador = EstadoJugador::Saltando;
        velocidadVertical = FUERZA_SALTO;
    }
    return true;
}

void mapa1::update(float delta) {
    // Actualizar temporizador
    tiempoRestante -= delta;
    labelTiempo->setString("Tiempo: " + std::to_string((int)tiempoRestante));

    if (tiempoRestante <= 0) {
        detenerMusica();
        cocos2d::AudioEngine::play2d("death_sound.mp3"); // Añadir el sonido de muerte
        auto gameOverScene = GameOverScene::createScene(1);
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
    }

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

    if (!metaAlcanzada && jugador->getBoundingBox().intersectsRect(meta->getBoundingBox())) {
        metaAlcanzada = true;

        // transición al menú 
        this->scheduleOnce([](float) {
            auto menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
            }, 1.0f, "transitionToMenu");

        return;
    }

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
            float nuevaAltura;

            if (plataforma->getTag() == 1) {
                nuevaAltura = plataforma->getPositionY() +
                    (plataforma->getContentSize().height * plataforma->getScaleY() * 0.5f);
            }
            else {
                nuevaAltura = plataforma->getPositionY() +
                    (plataforma->getContentSize().height * 0.5f);
            }

            jugador->setPositionY(nuevaAltura);
            estadoJugador = EstadoJugador::EnPlataforma;
            velocidadVertical = 0;
            break;
        }
    }

    // Si no está sobre ninguna plataforma, cambia a cayendo
    if (!jugadorSobrePlataforma && estadoJugador == EstadoJugador::EnPlataforma) {
        estadoJugador = EstadoJugador::Cayendo;
    }

    // Verificar colisiones con obstáculos
    for (auto& obstaculo : obstaculos) {
        auto obstaculoBoundingBox = obstaculo->getBoundingBox();
        if (jugadorBoundingBox.intersectsRect(obstaculoBoundingBox)) {
            if (obstaculo->getTag() == 2) { // Obstáculo que hace saltar
                if (estadoJugador == EstadoJugador::EnPlataforma) {
                    estadoJugador = EstadoJugador::Saltando;
                    velocidadVertical = FUERZA_SALTO*2.0f;
                    CCLOG("El jugador saltó por el obstáculo de salto.");
                }
            }
            else {
                detenerMusica();
                cocos2d::AudioEngine::play2d("death_sound.mp3"); // Añadir el sonido de muerte
                auto gameOverScene = GameOverScene::createScene(1);
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
            }
        }
    }

    
}

void mapa1::detenerMusica() {
    if (idMusicaFondo != cocos2d::AudioEngine::INVALID_AUDIO_ID) {
        cocos2d::AudioEngine::stop(idMusicaFondo);
    }
}


void mapa1::menuCerrarCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}





