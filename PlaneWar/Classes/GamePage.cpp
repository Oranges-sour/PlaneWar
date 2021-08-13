#include <vector>
using namespace std;
#include <cstdio>

#include "header/Enemy.h"
#include "header/EnemyGroup.h"
#include "header/GameLobby.h"
#include "header/GameManager.h"
#include "header/GameOverPage.h"
#include "header/GamePage.h"
#include "header/Hero.h"
#include "header/Joystick.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/ObjectPool.h"
#include "header/OptionPage.h"
#include "header/PhysicsShapeCache.h"
#include "header/Random.h"
#include "header/Weapon.h"
#include "header/XMLManager.h"

USING_NS_CC;

constexpr int HERO_MOVE_SPEED = 525;
constexpr int HERO_HIT_POINT = 150;
constexpr int HERO_ENERGY_POINT = 200;

enum _zORDER_Layer0 {
    z_bkground = 0,
    z_gameManager = 1,
};

enum _zORDER_Layer1 {
    z_blur_gameBK,
    z_count_down_timer,
    z_info,
    z_button,

    z_loadingbkground,
    z_block,
    z_block_out,

    z_blackbkground_top
};

Scene* GamePage::createScene()
{
    auto scene = GamePage::create();
    auto phy = scene->getPhysicsWorld();
    // phy->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // phy->setUpdateRate(2);
    phy->setGravity(Vec2::ZERO);
    return scene;
}

bool GamePage::init()
{
    layer0 = GamePageLayer0::create();
    this->addChild(layer0, 0);

    layer1 = GamePageLayer1::create();
    this->addChild(layer1, 1);

    MyDefault.gameState.isOnGame = true;

    return true;
}

void GamePage::onExit()
{
    MyDefault.gameState.isOnGame = false;
    Scene::onExit();
}

bool GamePageLayer0::init()
{
    if (!Layer::init()) {
        return false;
    }
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    // auto const texture = Director::getInstance()->getTextureCache();
    // auto const frameCache = SpriteFrameCache::getInstance();
    //////////////////////////////////
    MyDefault.gameState.enemyKilled = 0;

    //创建游戏管理器
    GameManager::createInstance();

    this->addChild(GameManager::getInstance(), _zORDER_Layer0::z_gameManager);
    GameManager::getInstance()->setPosition(Vec2::ZERO);

    GameManager::getInstance()->createGameBkground(Imgs.gamePage0.t_gameBK);

    GameManager::getInstance()->createGameMap(Imgs.gamePage0.t_gameMap);

    GameManager::getInstance()->createHero(Imgs.onGame.hero.p_hero,
                                           Imgs.onGame.hero.p_cannon,
                                           Imgs.onGame.hero.vp_explode, "hero");

    GameManager::getInstance()->createEnemyPointer(
        Imgs.gamePage0.p_enemyPointer);

    GameManager::getInstance()->createRedBK(Imgs.gamePage0.t_gameRedBK);

    GameManager::getInstance()->createJoystick(
        Imgs.gamePage0.joystick.vp_move, Imgs.gamePage0.joystick.vp_attack,
        Imgs.gamePage0.joystick.vp_skill);

    GameManager::getInstance()->createBar(
        Imgs.gamePage0.vp_bar[0], Imgs.gamePage0.vp_bar[1],
        Imgs.gamePage0.vp_bar[2], Imgs.gamePage0.vp_bar[3],
        Imgs.gamePage0.vp_bar[4], MyDefault.font.bold);

    GameManager::getInstance()->setBarPosition(
        Vec2(208, visibleSize.height - 113));

    //初始化英雄
    {
        hero = GameManager::getInstance()->getHero();

        //创建技能
        auto skill = HeroSkill_shield::create();
        skill->setMaxHitPoint(400);
        skill->setFullCountDown(18);
        skill->setIsFinishCountDown(true);

        hero->setSkill(skill);

        hero->setMaxHitPoint(HERO_HIT_POINT);
        hero->setMaxEnergyPoint(HERO_ENERGY_POINT);
        hero->setHitPoint(HERO_HIT_POINT);
        hero->setEnergyPoint(HERO_ENERGY_POINT);

        hero->setMoveSpeed(HERO_MOVE_SPEED);
        //创建英雄的子弹
        {
            auto weapon = Weapon_Hero02::create();
            weapon->setBulletInfo(
                Imgs.onGame.heroBullet0.p_bullet,
                Imgs.onGame.heroBullet0.vp_explode,
                Imgs.onGame.heroBullet0.vp_move, "hero_bullet",
                MyDefault.hero_bullet02.moveSpeed,
                MyDefault.hero_bullet02.particleLiveTime_OnMove,
                MyDefault.hero_bullet02.particleCreateSpeed_OnMove,
                MyDefault.hero_bullet02.particleLiveTime_OnExplode,
                MyDefault.hero_bullet02.particleQuantity_OnExplode,
                MyDefault.hero_bullet02.damage *
                    (MyDefault.updateInfo.attackDamage.nowValue / 100 + 1),
                MyDefault.hero_bullet02.energy, MyDefault.hero_bullet02.range,
                MyDefault.hero_bullet02.angleRotateSpeed,
                MyDefault.hero_bullet02.attackInterval *
                    (1 - MyDefault.updateInfo.attackDamage.nowValue / 100));

            hero->setWeaponAndAddChild(weapon);
        }
    }

    //初始化操纵杆
    {
        joystick = GameManager::getInstance()->getJoystick();
        joystick->setMoveOrgPos(Vec2(250, 250));
        joystick->setAttackPos(Vec2(visibleSize.width - 250, 250));
        joystick->setSkillPos(Vec2(visibleSize.width - 250, 500));
    }

    //初始化触摸监听
    {
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesEnded = [&](vector<Touch*> touch,
                                       Event* event) -> void {
            for (auto& t : touch) {
                TouchesPool::getInstance()->removeFromPool(t);
            }
        };

        listener->onTouchesBegan = [&](vector<Touch*> touch,
                                       Event* event) -> bool {
            for (auto& t : touch) {
                TouchesPool::getInstance()->addToPool(t);
            }
            return true;
        };

        listener->onTouchesMoved = [&](vector<Touch*> touch, Event* event) {

        };

        Director::getInstance()
            ->getEventDispatcher()
            ->addEventListenerWithSceneGraphPriority(listener, this);
    }

    //初始化碰撞检测
    {
        auto conactListener = EventListenerPhysicsContact::create();
        conactListener->onContactBegin = [&](PhysicsContact& contact) -> bool {
            GameManager::getInstance()->processContact(contact);
            return false;
        };
        conactListener->onContactSeparate = [&](PhysicsContact& contact) {
            return;
        };
        Director::getInstance()
            ->getEventDispatcher()
            ->addEventListenerWithSceneGraphPriority(conactListener, this);
    }

    //检测英雄是否死亡
    this->schedule(
        [&](float) {
            auto gameMana = GameManager::getInstance();
            if (gameMana->getHero()->getHitPoint() <= 0) {
                gameMana->onHeroDie();
                dynamic_cast<GamePage*>(this->getParent())
                    ->layer1->pause_button->setEnabled(false);
                this->unschedule("__checkHeroAlive");
                this->scheduleOnce(
                    [&](float) {
                        dynamic_cast<GamePage*>(this->getParent())
                            ->layer1->heroDie();
                    },
                    1.5f, "__die");
            }
        },
        "__checkHeroAlive");

    //英雄蓝太少自动减血
    this->schedule(
        [&](float) {
            auto gameMana = GameManager::getInstance();
            auto hero = gameMana->getHero();
            auto enemies =
                gameMana->getEnemyInRange(hero->getPosition(), FLT_MAX - 1);

            bool needSub = true;
            for (auto& it : enemies) {
                if (it->getTag() == tag_exp) {
                    needSub = false;
                    break;
                }
            }
            if (hero->getEnergyPoint() <= 2 && needSub) {
                hero->setHitPoint(hero->getHitPoint() - 5);
            }
        },
        0.5f, "__subHitPoint");

    //////////////////////////////////
    GameManager::getInstance()->setIsPause(false);
    GameManager::getInstance()->startFirstEnemyWave();
    return true;
}

void GamePageLayer0::onExit()
{
    TouchesPool::getInstance()->removeAllObjects();
    GameManager::destoryInstance();
    Layer::onExit();
}

bool GamePageLayer1::init()
{
    if (!Layer::init()) {
        return false;
    }
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    auto const texture = Director::getInstance()->getTextureCache();
    //////////////////////////////////

    blackBkground_top = Sprite::createWithTexture(
        texture->getTextureForKey(Imgs.share.t_blackBkGround));
    blackBkground_top->setPosition(visibleSize / 2);
    {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create(
            [this]() { this->pause_button->setEnabled(true); });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground_top->runAction(s);
    }
    this->addChild(blackBkground_top, _zORDER_Layer1::z_blackbkground_top);

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, _zORDER_Layer1::z_button);

    pause_button = [this]() -> MenuItemToggle*  //创建暂停按钮
    {
        auto ms0 = []() -> MenuItemSprite* {
            auto s0 =
                Sprite::createWithSpriteFrameName("game_pause_button0.png");
            auto s1 =
                Sprite::createWithSpriteFrameName("game_pause_button1.png");

            return MenuItemSprite::create(s0, s1);
        }();

        auto ms1 = []() -> MenuItemSprite* {
            auto s0 =
                Sprite::createWithSpriteFrameName("game_start_button0.png");
            auto s1 =
                Sprite::createWithSpriteFrameName("game_start_button1.png");

            return MenuItemSprite::create(s0, s1);
        }();

        return MenuItemToggle::createWithCallback([this](Ref*) { return; }, ms0,
                                                  ms1, nullptr);
    }();
    pause_button->setEnabled(false);
    pause_button->setPosition(Vec2(1811, visibleSize.height - 113));
    pause_button->setCallback([&](Ref*) {
        switch (pause_button->getSelectedIndex()) {
            case 0: {
                hideMenu();
                break;
            }
            case 1: {
                TouchesPool::getInstance()->removeAllObjects();
                pause_button->setEnabled(false);
                showMenu();
                break;
            }
        }
    });
    menu->addChild(pause_button);
    this->addButton(pause_button);

    info = Sprite::createWithTexture(
        texture->getTextureForKey("game_info_bkground.png"));
    info->setPosition(visibleSize.width / 2, visibleSize.height - 544);
    info->setVisible(false);
    this->addChild(info, _zORDER_Layer1::z_info);

    //创建“主菜单”按钮
    {
        auto s0 =
            Sprite::createWithSpriteFrameName("game_menu_mainPage_button0.png");
        auto s1 =
            Sprite::createWithSpriteFrameName("game_menu_mainPage_button1.png");
        info_mainPage =
            MenuItemSprite::create(s0, s1, [&](Ref*) { show_cancel(); });
        info_mainPage->setPosition(visibleSize.width / 2,
                                   visibleSize.height - 620);
        info_mainPage->setEnabled(false);
        info_mainPage->setVisible(false);
        menu->addChild(info_mainPage);
        this->addButton(info_mainPage);
    }

    //创建同意按钮
    {
        auto s0 = Sprite::createWithSpriteFrameName(
            "game_menu_mainPage_accept_button0.png");
        auto s1 = Sprite::createWithSpriteFrameName(
            "game_menu_mainPage_accept_button1.png");

        info_mainPage_accept = MenuItemSprite::create(s0, s1, [&](Ref*) {
            this->disableAllButtons();

            auto f = FadeIn::create(MyDefault.act.time);
            auto act = EaseIn::create(f, MyDefault.act.rate);
            auto func = CallFunc::create([]() {
                auto sence = GameOver::createScene();
                Director::getInstance()->replaceScene(sence);
            });
            auto s = Sequence::create(act, func, nullptr);
            blackBkground_top->runAction(s);
        });
        info_mainPage_accept->setPosition(visibleSize.width / 2,
                                          visibleSize.height - 540);
        info_mainPage_accept->setEnabled(false);
        info_mainPage_accept->setVisible(false);
        menu->addChild(info_mainPage_accept);
        this->addButton(info_mainPage_accept);
    }

    //创建取消按钮
    {
        auto s0 = Sprite::createWithSpriteFrameName(
            "game_menu_mainPage_cancel_button0.png");
        auto s1 = Sprite::createWithSpriteFrameName(
            "game_menu_mainPage_cancel_button1.png");

        info_mainPage_cancel =
            MenuItemSprite::create(s0, s1, [&](Ref*) { hide_cancel(); });
        info_mainPage_cancel->setPosition(visibleSize.width / 2,
                                          visibleSize.height - 700);
        info_mainPage_cancel->setEnabled(false);
        info_mainPage_cancel->setVisible(false);
        menu->addChild(info_mainPage_cancel);
        this->addButton(info_mainPage_cancel);
    }

    //////////////////////////////////
    return true;
}

void GamePageLayer1::onExit() { Layer::onExit(); }

void GamePageLayer1::showMenu()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    auto getBlurGameBk = [&](float blurR, float blurNum) -> Sprite* {
        auto cutGameScreen = [&]() -> RenderTexture* {
            auto t =
                RenderTexture::create(visibleSize.width, visibleSize.height);
            t->begin();
            dynamic_cast<GamePage*>(this->getParent())->layer0->visit();
            t->end();
            return t;
        };

        auto getBlurSprite = [&](RenderTexture* t) -> Sprite* {
            auto sf = t->getSprite()->getTexture();
            auto bs = Sprite::createWithTexture(sf);
            bs->setPosition(visibleSize / 2);
            auto p = GLProgramCache::getInstance()->getGLProgram("blur");
            auto ps = GLProgramState::getOrCreateWithGLProgram(p);

            auto size = bs->getTexture()->getContentSizeInPixels();
            ps->setUniformVec2("resolution", size / 16);
            ps->setUniformFloat("blurRadius", blurR);
            ps->setUniformFloat("sampleNum", blurNum);
            // ps->setUniformTexture("", nullptr);

            bs->setGLProgram(p);
            bs->setGLProgramState(ps);

            return bs;
        };

        auto getBlurTexture = [&](Sprite* s) -> RenderTexture* {
            auto t =
                RenderTexture::create(visibleSize.width, visibleSize.height);
            t->begin();
            s->visit();
            t->end();
            return t;
        };

        auto getResultSprite = [&](RenderTexture* t) -> Sprite* {
            return Sprite::createWithSpriteFrame(
                t->getSprite()->getSpriteFrame());
        };

        //////////////////////////////////////////////////////////////////////////
        auto t0 = cutGameScreen();
        auto t1 = getBlurSprite(t0);
        auto t2 = getBlurTexture(t1);
        return getResultSprite(t2);
    };

    if (blurGameBk) {
        blurGameBk->removeFromParent();
        blurGameBk = nullptr;
    }
    this->blurGameBk = getBlurGameBk(2.5f, 12.0f);

    this->blurGameBk->setOpacity(0);
    this->blurGameBk->setPosition(visibleSize / 2);

    auto f = FadeIn::create(MyDefault.act.time);
    auto act = EaseIn::create(f, MyDefault.act.rate);
    blurGameBk->runAction(act);

    this->addChild(blurGameBk, _zORDER_Layer1::z_blur_gameBK);

    info->setScale(0);
    info->setVisible(true);

    info_mainPage->setScale(0);
    info_mainPage->setOpacity(255);
    info_mainPage->setVisible(true);
    info_mainPage->setEnabled(true);

    GameManager::getInstance()->setIsPause(true);

    auto s = ScaleTo::create(MyDefault.act.time, 1.0f);
    auto act0 = EaseIn::create(s, MyDefault.act.rate);

    auto func = CallFunc::create([&]() { pause_button->setEnabled(true); });
    info->stopAllActions();
    info->runAction(Sequence::create(act0, func, nullptr));

    info_mainPage->stopAllActions();
    info_mainPage->runAction(act0->clone());
}

void GamePageLayer1::hideMenu()
{
    pause_button->setEnabled(false);

    info_mainPage->setEnabled(false);

    info_mainPage_accept->setEnabled(false);
    info_mainPage_accept->setVisible(false);

    info_mainPage_cancel->setEnabled(false);
    info_mainPage_cancel->setVisible(false);

    showCountDownTimer();
    [&]()  //创建背景的动画
    {
        if (blurGameBk) {
            auto f = FadeOut::create(MyDefault.act.time);
            auto act = EaseIn::create(f, MyDefault.act.rate);
            auto func = CallFunc::create([&]() {
                // GameManager::getInstance()->setIsPause(false);

                blurGameBk->removeFromParent();
                blurGameBk = nullptr;
            });
            auto s = Sequence::create(act, func, nullptr);
            blurGameBk->runAction(s);
        }
    }();

    auto s = ScaleTo::create(MyDefault.act.time, 0.0f);
    auto act0 = EaseIn::create(s, MyDefault.act.rate);
    auto func0 = CallFuncN::create([&](Node* n) {
        if (n) {
            n->setVisible(false);
        }
    });
    auto run = Sequence::create(act0, func0, nullptr);

    info->stopAllActions();
    info->runAction(run);

    info_mainPage->stopAllActions();
    info_mainPage->runAction(run->clone());
}

void GamePageLayer1::heroDie()
{
    pause_button->setEnabled(false);

    auto f = FadeIn::create(MyDefault.act.time);
    auto act = EaseIn::create(f, MyDefault.act.rate);
    auto func = CallFunc::create([]() {
        auto sence = GameOver::createScene();
        Director::getInstance()->replaceScene(sence);
    });
    auto s = Sequence::create(act, func, nullptr);
    blackBkground_top->runAction(s);
}

void GamePageLayer1::show_cancel()
{
    {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([&]() {
            info_mainPage->setVisible(false);

            info_mainPage_cancel->setEnabled(true);
            info_mainPage_accept->setEnabled(true);
        });
        auto r = Sequence::create(act, func, nullptr);
        info_mainPage->runAction(r);
        info_mainPage->setEnabled(false);
    }

    {
        info_mainPage_cancel->setVisible(true);
        info_mainPage_cancel->setOpacity(0);
        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        info_mainPage_cancel->runAction(act);
    }

    {
        info_mainPage_accept->setVisible(true);
        info_mainPage_accept->setOpacity(0);
        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        info_mainPage_accept->runAction(act);
    }
}

void GamePageLayer1::hide_cancel()
{
    {
        info_mainPage->setVisible(true);
        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        info_mainPage->runAction(act);
    }

    {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([&]() {
            info_mainPage_cancel->setVisible(false);

            info_mainPage->setEnabled(true);
        });
        auto r = Sequence::create(act, func, nullptr);
        info_mainPage_cancel->runAction(r);
        info_mainPage_cancel->setEnabled(false);
    }

    {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create(
            [&]() { info_mainPage_accept->setVisible(false); });
        auto r = Sequence::create(act, func, nullptr);
        info_mainPage_accept->runAction(r);
        info_mainPage_accept->setEnabled(false);
    }
}

void GamePageLayer1::showCountDownTimer()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    countDownTimer =
        Label::createWithTTF("", MyDefault.font.bold, 180, Size(650, 0),
                             TextHAlignment::CENTER, TextVAlignment::CENTER);
    countDownTimer->setPosition(visibleSize.width / 2,
                                visibleSize.height / 4 * 3 + 50);
    this->addChild(countDownTimer, _zORDER_Layer1::z_count_down_timer);

    countDownTimerMessage =
        Label::createWithTTF("", MyDefault.font.bold, 30, Size(650, 0),
                             TextHAlignment::CENTER, TextVAlignment::CENTER);
    countDownTimerMessage->setPosition(visibleSize.width / 2,
                                       visibleSize.height / 4 * 3 - 50);
    this->addChild(countDownTimerMessage, _zORDER_Layer1::z_count_down_timer);
    countDownTimerMessage->setString(
        XMLManager::getString("count_down_timer_mes"));

    this->schedule(
        [&](float) {
            char str[4] = "";
            std::snprintf(str, sizeof(str), "%d", 3 - timer_count);
            countDownTimer->setString(str);

            if (timer_count == 3) {
                this->unschedule("__count_down");
                pause_button->setEnabled(true);

                GameManager::getInstance()->setIsPause(false);
                timer_count = 0;
                auto start_str = XMLManager::getString("game_start");
                countDownTimer->setString(start_str);

                auto f = FadeOut::create(1.5f);
                auto func =
                    CallFuncN::create([&](Node* r) { r->removeFromParent(); });

                auto visibleSize = Director::getInstance()->getVisibleSize();
                auto act = Sequence::create(f, func, nullptr);
                countDownTimer->setPosition(visibleSize.width / 2 + 40,
                                            visibleSize.height / 4 * 3 + 50);
                countDownTimer->runAction(act);
                countDownTimerMessage->runAction(act->clone());
                //并未清除,只是取消了托管
                countDownTimer = nullptr;
                countDownTimerMessage = nullptr;

                return;
            }
            timer_count += 1;
        },
        0.6f, CC_REPEAT_FOREVER, MyDefault.act.time + 0.05f, "__count_down");
}
