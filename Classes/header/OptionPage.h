#ifndef OPTION_PAGE_H
#define OPTION_PAGE_H

#include "UI_Option.h"
#include "cocos2d.h"
USING_NS_CC;

#include "base_Page.h"

class OptionPage : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(OptionPage);
};

class OptionPageLayer0 : public base_Page {
public:
    virtual bool init();

    CREATE_FUNC(OptionPageLayer0);

private:
    //执行设置内容
    void setOption();

    cocos2d::Sprite* blackBkground = nullptr;

    //显示设置
    UI_OptionBar* showBar = nullptr;

    UI_OptionBarChild_Switch_2Buttons* showBar_highFrame = nullptr;
    UI_OptionBarChild_Switch_3Buttons* showBar_particleQuality = nullptr;
};

#endif