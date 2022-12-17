#include "AppDelegate.h"

#include "header/GameManager.h"
#include "header/Joystick.h"
#include "header/LoadingPage.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/ObjectPool.h"
#include "header/Random.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error \
    "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

// static cocos2d::Size smallResolutionSize = cocos2d::Size(1280, 720);
// static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
// static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes:
    // red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int init()
{
    MyMath::setScreenRect();

    auto UD = UserDefault::getInstance();
    if (!UD->getBoolForKey("isCreateBefore")) {
        UD->setBoolForKey("isCreateBefore", true);
        UD->setBoolForKey("option_highFrame", true);
        UD->setIntegerForKey("option_particleQuality", 2);
    }
    MyDefault.option.highFrame = UD->getBoolForKey("option_highFrame");
    MyDefault.option.particleQuality =
        UD->getIntegerForKey("option_particleQuality");
    MyDefault.updateInfo.attackSpeed.nowLevel =
        UD->getIntegerForKey("update_attackSpeed_level");
    MyDefault.updateInfo.attackDamage.nowLevel =
        UD->getIntegerForKey("update_attackDamage_level");
    MyDefault.updateInfo.defendSubDamage.nowLevel =
        UD->getIntegerForKey("update_defendSubDamage_level");
    MyDefault.gameState.money = UD->getIntegerForKey("money");
    MyDefault.gameState.exp_level = UD->getIntegerForKey("exp_level");
    MyDefault.gameState.exp_value = UD->getIntegerForKey("exp_value");

    Random::init();

    return 0;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||   \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(
            "Plane", cocos2d::Rect(0, 0, 1920 /*/ 2.5*/, 1080 /*/ 2.5*/));
#else
        glview = GLViewImpl::create("Plane");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    init();

    // set FPS. the default value is 1.0/60 if you don't call this
    if (MyDefault.option.highFrame)
        director->setAnimationInterval(1.0f / 60);
    else
        director->setAnimationInterval(1.0f / 30);

    // Set the design resolution
    glview->setDesignResolutionSize(MyDefault.designResolutionSize.width,
                                    MyDefault.designResolutionSize.height,
                                    ResolutionPolicy::SHOW_ALL);
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
            director->setContentScaleFactor(MIN(largeResolutionSize.height /
designResolutionSize.height, largeResolutionSize.width /
designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
            director->setContentScaleFactor(MIN(mediumResolutionSize.height /
designResolutionSize.height, mediumResolutionSize.width /
designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {
            director->setContentScaleFactor(MIN(smallResolutionSize.height /
designResolutionSize.height, smallResolutionSize.width /
designResolutionSize.width));
    }
#endif

     */

    director->setProjection(Director::Projection::_2D);

    // create a scene. it's an autorelease object
    auto scene = LoadingPage::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a
// phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    if (MyDefault.gameState.isOnGame == true) {
        TouchesPool::getInstance()->removeAllObjects();
        GameManager::getInstance()->getJoystick()->removeMoveTouchCatched();
        GameManager::getInstance()->getJoystick()->removeSkillTouchCatched();
    }
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
