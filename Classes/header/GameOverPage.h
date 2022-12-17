#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "base_Page.h"
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;

class GameOver : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
};

class GameOverLayer0 : public base_Page {
public:
    virtual bool init();

    CREATE_FUNC(GameOverLayer0);

private:
    cocos2d::Sprite* bkground = nullptr;
    cocos2d::Sprite* blackBkground = nullptr;
};

#endif
