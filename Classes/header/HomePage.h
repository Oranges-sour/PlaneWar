#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "base_Page.h"

class HomePage : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HomePage);
};

class HomePageLayer0 : public base_Page {
public:
    virtual bool init();

    CREATE_FUNC(HomePageLayer0);

private:
    cocos2d::Sprite* blackBkground = nullptr;
};

#endif