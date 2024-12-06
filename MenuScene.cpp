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

    // Crear un fondo de color sólido
    auto fondoColor = LayerColor::create(Color4B(100, 100, 255, 255));  // Fondo de color azul
    this->addChild(fondoColor);

    // Crear el botón "Mapa 1"
    auto botonMapa1 = ui::Button::create(); // Botón sin imágenes
    botonMapa1->setTitleText("Mapa 1");
    botonMapa1->setTitleFontSize(24);
    botonMapa1->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height / 4 + origen.y));
    botonMapa1->addClickEventListener([](Ref* sender) {
        CCLOG("Botón 'Mapa 1' clicado."); // Depuración
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


