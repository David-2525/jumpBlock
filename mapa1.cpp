#include "mapa1.h"
#include "GameOverScene.h"
#include "MenuScene.h"
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

    estaSaltando = false;
    estaEnPlataforma = false;

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
    plataformaPrincipal->setScale((tamanoVisible.width * 10) / plataformaPrincipal->getContentSize().width, 1.0f);
    plataformaPrincipal->setPosition(Vec2(tamanoVisible.width * 1.5f + origen.x, tamanoVisible.height * 0.1f + origen.y));
    this->addChild(plataformaPrincipal, 0);

    plataformas.pushBack(plataformaPrincipal);

    // Crear plataformas adicionales
    std::vector<Vec2> posicionesPlataformas = {
        Vec2(tamanoVisible.width * 1.8f + origen.x, tamanoVisible.height * 0.27f + origen.y),
        Vec2(tamanoVisible.width * 2.6f + origen.x, tamanoVisible.height * 0.4f + origen.y),
        Vec2(tamanoVisible.width * 3.5f + origen.x, tamanoVisible.height * 0.6f + origen.y)
    };

    for (const auto& posicion : posicionesPlataformas) {
        auto plataforma = Sprite::create("platform.png");
        plataforma->setScale(1.1f, 0.6f);
        plataforma->setPosition(posicion);

        // Etiquetar como plataforma secundaria
        plataforma->setTag(1); // 1 = plataforma secundaria
        this->addChild(plataforma, 0);
        plataformas.pushBack(plataforma);
    }

    // Crear el jugador sobre la plataforma principal
    jugador = Sprite::create("player.png");
    jugador->setPosition(Vec2(
        tamanoVisible.width * 0.2f + origen.x,
        plataformaPrincipal->getPositionY() + plataformaPrincipal->getContentSize().height / 2
    ));
    jugador->setScale(1.0f);
    this->addChild(jugador, 10);

    // Crear el mapa con obstáculos
    crearMapaConObstaculos();

    // Movimiento continuo
    float velocidad = 300.0f;

    this->schedule([=](float delta) mutable {
        fondo->setPositionX(fondo->getPositionX() - velocidad * delta);
        fondoCopia->setPositionX(fondoCopia->getPositionX() - velocidad * delta);

        if (fondo->getPositionX() <= -tamanoVisible.width / 2) {
            fondo->setPositionX(fondoCopia->getPositionX() + tamanoVisible.width);
        }
        if (fondoCopia->getPositionX() <= -tamanoVisible.width / 2) {
            fondoCopia->setPositionX(fondo->getPositionX() + tamanoVisible.width);
        }

        for (auto& plataforma : plataformas) {
            plataforma->setPositionX(plataforma->getPositionX() - velocidad * delta);

            if (plataforma->getPositionX() < -tamanoVisible.width / 2) {
                plataforma->setPositionX(tamanoVisible.width * 1.5f);
            }
        }

        for (auto& obstaculo : obstaculos) {
            obstaculo->setPositionX(obstaculo->getPositionX() - velocidad * delta);

            if (obstaculo->getPositionX() < -tamanoVisible.width / 2) {
                obstaculo->setPositionX(tamanoVisible.width * 1.5f);
            }
        }
        }, "MovimientoContinuo");

    // Configurar salto al tocar pantalla
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->onTouchBegan = CC_CALLBACK_2(mapa1::alTocarPantalla, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);

    this->scheduleUpdate();

    cocos2d::AudioEngine::play2d("background_music.mp3", true);

    CCLOG("Mapa 1 inicializado correctamente.");
    return true;
}

void mapa1::crearMapaConObstaculos() {
    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    std::vector<Vec2> posicionesObstaculos = {
        Vec2(tamanoVisible.width * 0.6f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.0f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.3f + origen.x, plataformaPrincipal->getPositionY() + 65),
        Vec2(tamanoVisible.width * 1.9f + origen.x, plataformaPrincipal->getPositionY() + 170)
    };

    for (const auto& posicion : posicionesObstaculos) {
        auto obstaculo = Sprite::create("obstacle1.png");
        obstaculo->setPosition(posicion);
        obstaculo->setScale(0.2f, 0.2f);
        this->addChild(obstaculo);
        obstaculos.pushBack(obstaculo);
    }
}

bool mapa1::alTocarPantalla(Touch* toque, Event* evento) {
    if (!estaSaltando) {
        estaSaltando = true;
        estaEnPlataforma = false;

        // Aumentar la altura del salto para alcanzar plataformas más altas
        auto salto = JumpBy::create(1.0f, Vec2(0, 0), 200, 1); // Ajustar la altura si es necesario
        auto reiniciarEstado = CallFunc::create([=]() {
            estaSaltando = false;
            });

        jugador->runAction(Sequence::create(salto, reiniciarEstado, nullptr));
        cocos2d::AudioEngine::play2d("jump_sound_effect.mp3");
    }
    return true;
}


void mapa1::update(float delta) {
    auto jugadorBoundingBox = jugador->getBoundingBox();

    // Verificar si el jugador toca una plataforma
    bool jugadorEnPlataforma = false;
    for (auto& plataforma : plataformas) {
        auto plataformaBoundingBox = plataforma->getBoundingBox();

        // Ajustar el área de colisión de la plataforma
        Rect areaColisionPlataforma(
            plataformaBoundingBox.origin.x,
            plataformaBoundingBox.origin.y + plataformaBoundingBox.size.height * 0.5f, // Reducir área superior
            plataformaBoundingBox.size.width,
            plataformaBoundingBox.size.height * 0.5f // Reducir altura
        );

        // Verificar colisión con la parte superior de la plataforma
        if (jugadorBoundingBox.intersectsRect(areaColisionPlataforma) && jugador->getPositionY() > plataforma->getPositionY()) {
            jugadorEnPlataforma = true;

            // Ajustar posición del jugador para colocarlo justo encima
            if (!estaSaltando) {
                float nuevaAltura = plataforma->getPositionY() + plataforma->getContentSize().height / 2;
                jugador->setPositionY(nuevaAltura);
                estaEnPlataforma = true;
                CCLOG("Jugador aterrizó sobre una plataforma. Altura ajustada.");
            }

            // Actualizar posición horizontal del jugador si la plataforma se mueve
            if (jugadorEnPlataforma && estaEnPlataforma) {
                jugador->setPositionX(jugador->getPositionX() + (plataforma->getPositionX() - plataformaAnteriorX));
            }
            plataformaAnteriorX = plataforma->getPositionX();

            break;
        }
    }

    if (!jugadorEnPlataforma) {
        estaEnPlataforma = false;
    }

    // Verificar colisiones con obstáculos
    for (auto& obstaculo : obstaculos) {
        auto obstaculoBoundingBox = obstaculo->getBoundingBox();

        Rect areaColisionObstaculo(
            obstaculoBoundingBox.origin.x + 10,
            obstaculoBoundingBox.origin.y + 10,
            obstaculoBoundingBox.size.width - 20,
            obstaculoBoundingBox.size.height - 20
        );

        if (jugadorBoundingBox.intersectsRect(areaColisionObstaculo)) {
            CCLOG("Colisión detectada con obstáculo.");
            auto gameOverScene = GameOverScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
            return;
        }
    }

    // Manejar caída fuera del mapa
    if (jugador->getPositionY() < 0) {
        CCLOG("El jugador ha caído fuera del mapa.");
        auto gameOverScene = GameOverScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameOverScene));
    }
}


