#ifndef GAME_LOBBY_H
#define GAME_LOBBY_H

#include "Switch.h"
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;
#include "base_Page.h"

class UI_Store_updateBar_info;

class GameLobby : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameLobby);
};

class GameLobbyLayer0 : public base_Page {
public:
    virtual bool init();

    CREATE_FUNC(GameLobbyLayer0);

    void particleOnUpdate(const Vec2& pos);

private:
    Sprite* bkground = nullptr;
    Sprite* blackBkground = nullptr;

    UI_Store_updateBar_info* updateAttack_attackSpeed_info = nullptr;
    UI_Store_updateBar_info* updateAttack_attackDamage_info = nullptr;
    Label* moneyShow = nullptr;
};

#endif
