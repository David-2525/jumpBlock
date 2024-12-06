#include "GameOverScene.h"
#include "MenuScene.h"
#include "mapa1.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* GameOverScene::createScene() {
    return GameOverScene::create();
}

bool GameOverScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Fondo de color sólido para la escena de Game Over
    auto fondoColor = LayerColor::create(Color4B(100, 100, 100, 255));  // Fondo gris oscuro
    this->addChild(fondoColor);

    // Etiqueta de "Game Over"
    auto etiquetaGameOver = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
    etiquetaGameOver->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.75 + origen.y));
    this->addChild(etiquetaGameOver);

    // Crear un nodo de capa para manejar los botones
    auto layer = Layer::create();
    this->addChild(layer);

    // Botón para ir al menú principal
    auto botonMenuPrincipal = ui::Button::create();
    botonMenuPrincipal->setTitleText("Menu Principal");
    botonMenuPrincipal->setTitleFontSize(24);
    botonMenuPrincipal->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.5 + origen.y));
    botonMenuPrincipal->addClickEventListener([](Ref* sender) {
        CCLOG("Botón de menú principal clicado");
        auto menuScene = MenuScene::createScene();
        if (menuScene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
        }
        });
    layer->addChild(botonMenuPrincipal);

    // Botón para reintentar el mapa 1
    auto botonReintentar = ui::Button::create();
    botonReintentar->setTitleText("Reintentar");
    botonReintentar->setTitleFontSize(24);
    botonReintentar->setPosition(Vec2(tamanoVisible.width / 2 + origen.x, tamanoVisible.height * 0.35 + origen.y));
    botonReintentar->addClickEventListener([](Ref* sender) {
        CCLOG("Botón de reintentar clicado");
        auto mapa1Scene = mapa1::createScene();
        if (mapa1Scene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mapa1Scene));
        }
        });
    layer->addChild(botonReintentar);

   

    return true;
}

