#ifndef __HERO_SKILL_H__
#define __HERO_SKILL_H__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/UILoadingBar.h"
using namespace cocos2d::ui;

#include "MyDefault.h"
#include "PhysicsShapeCache.h"
#include "Switch.h"
#include "basic_Object.h"

enum class HeroSkillType {
    base,
    shield,
    knife,
};

class Weapon_HeroSkill;

class HeroSkill_base : public Node {
public:
    //只调用一次,开始技能
    virtual void runSkill() { switch_isRunSkill.setSwitch(true); }

    //停止技能
    virtual void stopSkill() { switch_isRunSkill.setSwitch(false); }

    //设置倒计时时间(满时间)
    virtual void setFullCountDown(float countDown)
    {
        fullCountDown = countDown;
    }

    virtual float getFullCountDown() { return fullCountDown; }

    //获得当前倒计时时间
    virtual float getCountDown() { return countDown; }

    virtual void setCountDown(float c) { countDown = c; }

    virtual bool isFinishCountDown()
    {
        return switch_isFinishCountDown.isSet();
    }

    virtual void setIsFinishCountDown(bool s)
    {
        switch_isFinishCountDown.setSwitch(s);
    }

    virtual bool isRunSkill() { return switch_isRunSkill.isSet(); }

    virtual HeroSkillType getSkillType() { return HeroSkillType::base; }

private:
    float countDown = 0;
    float fullCountDown = 0;
    Switch switch_isFinishCountDown;
    Switch switch_isRunSkill;
};

class HeroSkill_shield : public HeroSkill_base {
public:
    static HeroSkill_shield* create();

    //每次开始技能只调用一次
    virtual void runSkill() override;

    virtual void stopSkill() override;

    void _update();

    void setMaxHitPoint(float hitPoint);

    float getMaxHitPoint();

    virtual HeroSkillType getSkillType() override
    {
        return HeroSkillType::shield;
    }

private:
    void showParticleOnMove();

    void getAttack(float damage);

    void showParticleOnStart();

private:
    class Shield;

    float hitPoint = 0;
    float maxHitPoint = 0;

    Sprite* skillBK = nullptr;
    Shield* shield = nullptr;
    LoadingBar* bar = nullptr;
    Sprite* bar_bk = nullptr;
};

class HeroSkill_shield::Shield : public basic_Object {
public:
    static HeroSkill_shield::Shield* create(const string& frame);
    void setSkill(HeroSkill_shield* skill);

public:  // base_Object继承
    // virtual Vec2 getLastPosition()override;
    // virtual void setLastPosition(const Vec2& pos)override;
    // virtual void onContact(Node* target)override;
    // virtual void kill()override;
    virtual void getAttack(float damage) override;
    virtual void _update() override;

private:
    friend class HeroSkill_shield;
    HeroSkill_shield* skill = nullptr;
};

#if 1

//废弃的
class HeroSkill_knife : public HeroSkill_base {
};

#else
class HeroSkill_knife : public HeroSkill_base {
public:
    static HeroSkill_knife* create();

    //每次开始技能只调用一次
    virtual void runSkill() override;

    virtual void stopSkill() override;

    void setWeapon(Weapon_HeroSkill* weapon);

    void _update();

    virtual HeroSkillType getSkillType() override
    {
        return HeroSkillType::knife;
    }

private:
    LoadingBar* bar = nullptr;
    Sprite* bar_bk = nullptr;
    Sprite* skillBK = nullptr;

    Weapon_HeroSkill* weapon = nullptr;
    const float maxValue = 100;
    float nowValue = 0;
};

#endif

#endif