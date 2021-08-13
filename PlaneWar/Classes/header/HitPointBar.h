#ifndef __HIP_POINT_BAR__
#define __HIP_POINT_BAR__

#include <string>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;
#include "ui/UILoadingBar.h"
using namespace cocos2d::ui;

class base_attackEnemy;
class Hero;

class HitPointBar_forEnemy : public cocos2d::Node {
public:
    static HitPointBar_forEnemy* create(
        base_attackEnemy* enemy, const string& hitPointBar_Plist,
        const string& hitPointBar_background_Plist);

    void _update();

    bool init(base_attackEnemy* enemy);

    void fadeInBar();

    void fadeOutBar();

    virtual void setPosition(const Vec2& position) override;

private:
    base_attackEnemy* enemy = nullptr;
    LoadingBar* hitPointBar = nullptr;
    Sprite* hitPointBar_background = nullptr;
};

/*
class HitPointBar_Hero : public cocos2d::Node
{
public:
        static HitPointBar_Hero* create(const string& bar_front, const string&
energy, const string& hitpoint, const string& bar_bk); private: friend class
Hero; void setFullPoint(int hitPoint, int energyPoint); void setHitPoint(float
hp); void setEnergyPoint(float ep); int getHitPoint(); int getEnergyPoint();
        void setHero(Hero* hero);
        bool isAlive();
        void updateBar(float);
        virtual void removeFromParent()override;
        virtual bool init()override;

        float hp_should = 0;//应该的值
        float hp_now = 0;//当前值
        int fullHP = 0;//满值

        float ep_should = 0;
        float ep_now = 0;
        int fullEP = 0;

        LoadingBar* bar_hp = nullptr;
        LoadingBar* bar_ep = nullptr;

        Sprite* bar_front = nullptr;
        Sprite* bar_bk = nullptr;

        Hero* hero = nullptr;
};

*/

#endif