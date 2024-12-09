#include "MenuScene.h"
#include "mapa1.h"
#include "mapa2.h"
#include "mapa3.h"
#include "ui/CocosGUI.h"

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

    // Fondo de color sólido
    auto fondoColor = LayerColor::create(Color4B(100, 100, 255, 255));
    this->addChild(fondoColor);

    // Título del juego
    auto titulo = Label::createWithSystemFont("Jump Block", "Arial", 48);
    titulo->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.8f + origen.y));
    this->addChild(titulo);

    // Crear un botón básico para Mapa 1
    auto botonMapa1 = ui::Button::create();
    botonMapa1->setTitleText("Mapa 1");
    botonMapa1->setTitleColor(Color3B::WHITE);
    botonMapa1->setTitleFontSize(24);
    botonMapa1->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.65f + origen.y));
    botonMapa1->setContentSize(Size(200, 60));
    botonMapa1->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Iniciando Mapa 1");
            auto scene = mapa1::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
        }
        });
    this->addChild(botonMapa1);

    // Crear un botón básico para Mapa 2
    auto botonMapa2 = ui::Button::create();
    botonMapa2->setTitleText("Mapa 2");
    botonMapa2->setTitleColor(Color3B::WHITE);
    botonMapa2->setTitleFontSize(24);
    botonMapa2->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.5f + origen.y));
    botonMapa2->setContentSize(Size(200, 60));
    botonMapa2->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Iniciando Mapa 2");
            auto scene = mapa2::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
        }
        });
    this->addChild(botonMapa2);

    

    // Crear un botón básico para Salir
    auto botonSalir = ui::Button::create();
    botonSalir->setTitleText("Salir");
    botonSalir->setTitleColor(Color3B::WHITE);
    botonSalir->setTitleFontSize(24);
    botonSalir->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.2f + origen.y));
    botonSalir->setContentSize(Size(200, 60));
    botonSalir->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Cerrando juego");
            Director::getInstance()->end();
        }
        });
    this->addChild(botonSalir);

    CCLOG("MenuScene inicializado correctamente.");
    return true;
}