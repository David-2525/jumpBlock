#include "MenuScene.h"
#include "mapa1.h"
#include "ui/CocosGUI.h" // Para usar botones

USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init()) {
        CCLOG("Error: Fallo al inicializar la escena base.");
        return false;
    }

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Crear un fondo de color s�lido
    auto fondoColor = LayerColor::create(Color4B(100, 100, 255, 255));  // Fondo de color azul
    this->addChild(fondoColor);

    // Crear el bot�n "Mapa 1"
    auto botonMapa1 = ui::Button::create(); // Bot�n sin im�genes
    botonMapa1->setTitleText("Mapa 1");
    botonMapa1->setTitleFontSize(24);
    botonMapa1->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height / 4 + origen.y));
    botonMapa1->addClickEventListener([](Ref* sender) {
        CCLOG("Bot�n 'Mapa 1' clicado."); // Depuraci�n
        auto map1Scene = mapa1::createScene();
        if (map1Scene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, map1Scene));
        }
        else {
            CCLOG("Error: No se pudo crear la escena 'mapa1'.");
        }
        });
    this->addChild(botonMapa1);

    CCLOG("MenuScene inicializado correctamente.");
    return true;
}


