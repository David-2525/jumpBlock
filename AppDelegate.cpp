#include "AppDelegate.h"
#include "MenuScene.h" 
#include "mapa1.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs() {
    // set OpenGL context attributes: red, green, blue, alpha, depth, stencil, multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("EpicGame", cocos2d::Rect(0, 0, 1280, 720), 1.0f, true);
#else
        glview = GLViewImpl::create("EpicGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::NO_BORDER);
    glview->setFrameSize(1280, 720);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();  // Cambié a MenuScene para iniciar con el menú principal

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
