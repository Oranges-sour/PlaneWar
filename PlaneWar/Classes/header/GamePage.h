#ifndef GANE_PAGE_H
#define GANE_PAGE_H

#include "cocos2d.h"
USING_NS_CC;

#include <string>
using namespace std;

#include "Switch.h"
#include "base_Page.h"

class GamePageLayer0;
class GamePageLayer1;
class Hero;
class Joystick;
class HitPointBar_Hero;

class GamePage : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onExit() override;

    static GamePage* create()
    {
        auto s = new (std::nothrow) GamePage();
        if (s && s->initWithPhysics() && s->init()) {
            s->autorelease();
            return s;
        }
        delete s;
        return nullptr;
    }

    GamePageLayer0* layer0 = nullptr;
    GamePageLayer1* layer1 = nullptr;
};

class GamePageLayer0 : public cocos2d::Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(GamePageLayer0);
    virtual void onExit() override;

private:
    friend class GamePageLayer1;

    HitPointBar_Hero* bar = nullptr;
    Joystick* joystick = nullptr;
    Hero* hero = nullptr;
};

class GamePageLayer1 : public base_Page {
public:
    virtual bool init() override;
    virtual void onExit() override;

    CREATE_FUNC(GamePageLayer1);

    void showMenu();
    void hideMenu();

    //Ö´ÐÐÓ¢ÐÛËÀÍöºóµÄaction
    void heroDie();

private:
    void show_cancel();
    void hide_cancel();

    void showCountDownTimer();

private:
    friend class GamePageLayer0;

    Sprite* info = nullptr;

    MenuItemSprite* info_mainPage = nullptr;
    MenuItemSprite* info_mainPage_cancel = nullptr;
    MenuItemSprite* info_mainPage_accept = nullptr;

    MenuItemToggle* pause_button = nullptr;

    Sprite* blurGameBk = nullptr;

    Sprite* blackBkground_top = nullptr;

private:
    Label* countDownTimer = nullptr;
    Label* countDownTimerMessage = nullptr;
    int timer_count = 0;
};

#endif
