#include "GameOverScene.h"
#include "MenuScene.h"
#include "mapa1.h"
#include "mapa2.h"
#include "mapa3.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* GameOverScene::createScene(int mapaActual) {
    auto scene = GameOverScene::create();
    if (scene) {
        auto layer = dynamic_cast<GameOverScene*>(scene);
        if (layer) {
            layer->initWithMapa(mapaActual);
        }
    }
    return scene;
}

bool GameOverScene::init() {
    return Scene::init();
}

bool GameOverScene::initWithMapa(int mapaActual) {
    if (!Scene::init()) {
        return false;
    }

    mapaActual_ = mapaActual;

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Fondo de color sólido para la escena de Game Over
    auto fondoColor = LayerColor::create(Color4B(100, 100, 100, 255));
    this->addChild(fondoColor);

    // Etiqueta de "Game Over"
    auto etiquetaGameOver = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
    etiquetaGameOver->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.75 + origen.y));
    this->addChild(etiquetaGameOver);

    // Botón para regresar al menú principal
    auto botonMenuPrincipal = ui::Button::create();
    botonMenuPrincipal->setTitleText("Menu Principal");
    botonMenuPrincipal->setTitleFontSize(24);
    botonMenuPrincipal->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.5 + origen.y));
    botonMenuPrincipal->addClickEventListener([](Ref* sender) {
        CCLOG("Botón de menú principal clicado");
        auto menuScene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
        });
    this->addChild(botonMenuPrincipal);

    // Botón para reintentar el mapa actual
    auto botonReintentar = ui::Button::create();
    botonReintentar->setTitleText("Reintentar");
    botonReintentar->setTitleFontSize(24);
    botonReintentar->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.35 + origen.y));
    botonReintentar->addClickEventListener([this](Ref* sender) {
        Scene* retryScene = nullptr;
        if (mapaActual_ == 1) {
            retryScene = mapa1::createScene();
        }
        else if (mapaActual_ == 2) {
            retryScene = mapa2::createScene();
        }
        

        if (retryScene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, retryScene));
        }
        });
    this->addChild(botonReintentar);

    return true;
}




