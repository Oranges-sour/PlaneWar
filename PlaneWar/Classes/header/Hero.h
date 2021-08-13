#ifndef __HERO_H__
#define __HERO_H__

#include <string>
#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "HeroSkill.h"
#include "Switch.h"
#include "basic_Object.h"

class HitPointBar_Hero;
class Contact;
class base_Weapon;

class Hero : public basic_Object {
public:
    static Hero* create(const string& hero, const string& cannon);

    /*void createBar(const string& bar_front, const string& energy,
            const string& hitpoint, const string& bar_bk);*/

    // HitPointBar_Hero* const getBar();

    void shootBullet();

    void setSkill(HeroSkill_base* skill);

    HeroSkill_base* getSkill();

    void setWeaponAndAddChild(base_Weapon* weapon);

    float getAttackInterval();

    void setMoveSpeed(float v);
    float getMoveSpeed();

    Sprite* getCannon();

public:  //血量相关
    float getHitPoint();
    void setHitPoint(float p);
    float getMaxHitPoint();
    void setMaxHitPoint(float p);

    float getEnergyPoint();
    void setEnergyPoint(float p);
    float getMaxEnergyPoint();
    void setMaxEnergyPoint(float p);

public:  //父类继承
    virtual bool init() override;
    virtual void removeFromParent() override;

public:  // base_Object继承
    // virtual Vec2 getLastPosition()override;
    // virtual void setLastPosition(const Vec2& pos)override;
    // virtual void onContact(Node* target)override;
    virtual void kill() override;
    virtual void getAttack(float damage) override;
    virtual void _update() override;

private:
    float moveSpeed = 0;

private:  //血量能量相关
    float hp_now = 0;
    float hp_max = 0;

    float ep_now = 0;
    float ep_max = 0;

private:
    base_Weapon* weapon = nullptr;
    Sprite* cannon = nullptr;
    HeroSkill_base* skill = nullptr;
};

//英雄显示免伤
class Hero_showText : public Node {
public:
    static Hero_showText* create();

    virtual bool init() override;
    // virtual void setPosition(const Vec2 &position)override;
private:
    Label* label = nullptr;
};

#endif
