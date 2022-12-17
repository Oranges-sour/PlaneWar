#include "header/LoadingPage.h"

#include <string>
#include <vector>

#include "header/GameManager.h"
#include "header/HomePage.h"
#include "header/MyDefault.h"
#include "header/PhysicsShapeCache.h"
#include "header/XMLManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

enum _zORDER { z_bkground, z_plane, z_blur, z_blackbkground };

Scene* LoadingPage::createScene() { return LoadingPage::create(); }

bool LoadingPage::init()
{
    if (!Scene::create()) {
        return false;
    }

    auto layer0 = LoadingPageLayer0::create();
    this->addChild(layer0, 0);

    return true;
}

bool LoadingPageLayer0::init()
{
    if (!Layer::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    //////////////////////////////////

    loadingBkground = Sprite::create(Imgs.loadingPage.t_loadingBK);
    loadingBkground->setPosition(visibleSize / 2);  //屏幕中间
    this->addChild(loadingBkground, _zORDER::z_bkground);

    blackBkground = Sprite::create(Imgs.share.t_blackBkGround);
    blackBkground->setPosition(visibleSize / 2);
    [&]() {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        blackBkground->runAction(act);
    }();
    this->addChild(blackBkground, _zORDER::z_blackbkground);

    plane = Sprite::create(Imgs.loadingPage.t_plane);
    plane->setPosition(visibleSize / 2);
    this->addChild(plane, _zORDER::z_plane);

    //////////////////////////////////
    //_spriteFrameChche->removeSpriteFramesFromFile("loading.plist");

    this->schedule(  //检测是否加载完成
        [&](float) {
            if (isLoadFinish.isSet()) {
                this->unschedule("__check_finish");

                [&]() {
                    [this]() {
                        auto f = FadeIn::create(MyDefault.act.time);
                        auto act = EaseIn::create(f, MyDefault.act.rate);
                        auto func = CallFunc::create([]() {
                            auto sence = HomePage::createScene();
                            Director::getInstance()->replaceScene(sence);
                        });
                        auto s = Sequence::create(act, func, nullptr);
                        blackBkground->runAction(s);
                    }();
                }();
            }
        },
        "__check_finish");

    this->schedule(  //加载资源
        [&](float) {
            static int fileCount = 0;
            static auto const texture =
                Director::getInstance()->getTextureCache();
            static auto const frame = SpriteFrameCache::getInstance();
            static vector<string> file{
                "images0.png",
                "images1.png",

                Imgs.share.t_bkGround,
                Imgs.share.t_blackBkGround,

                Imgs.homePage.t_effect,

                Imgs.optionPage.t_infoBK,

                Imgs.gameLobby.t_cutLine,
                Imgs.gameLobby.t_updateAttackBK,
                Imgs.gameLobby.t_updateDefendBk,

                Imgs.gameOver.t_overBK,

                Imgs.gamePage0.t_gameBK,
                Imgs.gamePage0.t_gameMap,
                Imgs.gamePage0.t_gameRedBK,

                Imgs.gamePage1.t_pauseBK,

                Imgs.onGame.skillShield.t_background,
                Imgs.onGame.skillShield.t_shake,

            };
            static int fileCountMax = file.size();
            if (fileCount < fileCountMax) {
                texture->addImage(file[fileCount]);
                ++fileCount;
            } else {
                this->unschedule("__load_file");

                PhysicsShapeCache::getInstance()->addShapesWithFile(
                    "physicsInfo.plist");

                frame->addSpriteFramesWithFile(
                    "images0.plist", texture->getTextureForKey("images0.png"));
                frame->addSpriteFramesWithFile(
                    "images1.plist", texture->getTextureForKey("images1.png"));

                XMLManager::pushFileToManager("strings.xml");

                auto program = GLProgram::createWithFilenames(
                    shaderProgramFilePath("white.vsh"),
                    shaderProgramFilePath("white.fsh"));
                GLProgramCache::getInstance()->addGLProgram(program, "white");

                auto program1 = GLProgram::createWithFilenames(
                    shaderProgramFilePath("light.vsh"),
                    shaderProgramFilePath("light.fsh"));
                GLProgramCache::getInstance()->addGLProgram(program1, "light");

                auto program2 = GLProgram::createWithFilenames(
                    shaderProgramFilePath("blur.vsh"),
                    shaderProgramFilePath("blur.fsh"));
                GLProgramCache::getInstance()->addGLProgram(program2, "blur");

                auto program3 = GLProgram::createWithFilenames(
                    shaderProgramFilePath("outGlow.vsh"),
                    shaderProgramFilePath("outGlow.fsh"));
                GLProgramCache::getInstance()->addGLProgram(program3,
                                                            "outGlow");

                isLoadFinish.setSwitch(true);
            }
        },
        0, CC_REPEAT_FOREVER, 1.2f, "__load_file");

    return true;
}
