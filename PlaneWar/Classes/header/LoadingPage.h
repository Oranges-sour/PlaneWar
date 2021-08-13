#ifndef LOADING_PAGE_H
#define LOADING_PAGE_H

#include "cocos2d.h"
#include "ui/UILoadingBar.h"
USING_NS_CC;
using namespace cocos2d::ui;

#include "Switch.h"

class LoadingPage : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(LoadingPage);
};

class LoadingPageLayer0 : public cocos2d::Layer {
public:
    virtual bool init();

    CREATE_FUNC(LoadingPageLayer0);

private:
    Switch isLoadFinish;

private:
    cocos2d::Sprite* loadingBkground = nullptr;
    cocos2d::Sprite* blackBkground = nullptr;
    cocos2d::Sprite* plane = nullptr;
};

#endif
