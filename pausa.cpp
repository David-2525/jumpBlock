#include "pausa.h"
#include "MenuScene.h"
#include "mapa1.h"
#include "mapa2.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* pausa::createScene(Scene* escenaFondo) {
    // Crear una nueva escena semi-transparente
    auto escena = Scene::create();
    auto capa = pausa::create();

    // Guardar referencia a la escena anterior
    capa->escenaAnterior = escenaFondo;

    // Pausar la escena anterior
    if (escenaFondo) {
        escenaFondo->pause();
    }

    escena->addChild(capa);
    return escena;
}

bool pausa::init() {
    if (!Layer::init()) {
        return false;
    }

    auto tamanoVisible = Director::getInstance()->getVisibleSize();
    Vec2 origen = Director::getInstance()->getVisibleOrigin();

    // Crear fondo semi-transparente negro
    auto fondo = LayerColor::create(Color4B(0, 0, 0, 128));
    this->addChild(fondo);

    // Crear título de pausa
    auto etiquetaPausa = Label::createWithSystemFont("PAUSA", "Arial", 48);
    etiquetaPausa->setPosition(Vec2(
        origen.x + tamanoVisible.width / 2,
        origen.y + tamanoVisible.height * 0.7
    ));
    this->addChild(etiquetaPausa);

    // Crear menú de opciones
    auto etiquetaReanudar = Label::createWithSystemFont("Reanudar", "Arial", 36);
    auto itemReanudar = MenuItemLabel::create(
        etiquetaReanudar,
        CC_CALLBACK_1(pausa::menuReanudarCallback, this)
    );

    auto etiquetaReintentar = Label::createWithSystemFont("Reintentar", "Arial", 36);
    auto itemReintentar = MenuItemLabel::create(
        etiquetaReintentar,
        CC_CALLBACK_1(pausa::menuReintentarCallback, this)
    );

    auto etiquetaMenuPrincipal = Label::createWithSystemFont("Menu Principal", "Arial", 36);
    auto itemMenuPrincipal = MenuItemLabel::create(
        etiquetaMenuPrincipal,
        CC_CALLBACK_1(pausa::menuPrincipalCallback, this)
    );

    // Crear y configurar el menú
    auto menu = Menu::create(itemReanudar, itemReintentar, itemMenuPrincipal, nullptr);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(
        origen.x + tamanoVisible.width / 2,
        origen.y + tamanoVisible.height / 2
    ));

    this->addChild(menu);

    // Crear listener para prevenir clicks en la escena de fondo
    auto escuchador = EventListenerTouchOneByOne::create();
    escuchador->setSwallowTouches(true);
    escuchador->onTouchBegan = [](Touch* toque, Event* evento) {
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);

    return true;
}

void pausa::menuReanudarCallback(Ref* pSender) {
    // Reanudar la escena anterior
    if (escenaAnterior) {
        escenaAnterior->resume();
        AudioEngine::resumeAll();
    }

    // Quitar la escena de pausa
    Director::getInstance()->popScene();
}

void pausa::menuReintentarCallback(Ref* pSender) {
    // Reanudar el audio antes de cambiar de escena
    AudioEngine::resumeAll();

    // Crear nueva instancia de la escena actual
    Scene* nuevaEscena = nullptr;

    // Determinar qué tipo de escena es y crear la correspondiente
    if (dynamic_cast<mapa1*>(escenaAnterior)) {
        nuevaEscena = mapa1::createScene();
    }
    else if (dynamic_cast<mapa2*>(escenaAnterior)) {
        nuevaEscena = mapa2::createScene();
    }

    // Si se pudo crear la nueva escena, hacer la transición
    if (nuevaEscena) {
        Director::getInstance()->replaceScene(
            TransitionFade::create(0.5f, nuevaEscena)
        );
    }
}

void pausa::menuPrincipalCallback(Ref* pSender) {
    // Reanudar el audio antes de cambiar de escena
    AudioEngine::resumeAll();

    // Crear y transicionar al menú principal
    auto escena = MenuScene::createScene();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5f, escena)
    );
}