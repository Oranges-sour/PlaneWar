#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <string>
#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "Switch.h"

class Hero;

class Joystick : public cocos2d::Node {
public:
    //均为plist文件名
    static Joystick* create(const vector<string>& move,
                            const vector<string>& attack,
                            const vector<string>& skill);

    virtual bool init(const vector<string>& move, const vector<string>& attack,
                      const vector<string>& skill);

public:
    Vec2 getMoveAddPos();
    Vec2 getSkillAddPos();
    Vec2 getAttackAddPos();
    void setMoveOrgPos(const Vec2& pos);

    //当前Touch要被release, move取消追踪坐标
    void removeMoveTouchCatched();
    void removeSkillTouchCatched();
    void removeAttackTouchCatched();
    void setAttackPos(const Vec2& pos);
    void setSkillPos(const Vec2& pos);

private:
    class _joystick_move;
    class _joystick_attack;
    class _joystick_skill;

private:
    _joystick_move* move;
    _joystick_attack* attack;
    _joystick_skill* skill;
};

class Joystick::_joystick_move : public Node {
public:
    static _joystick_move* create(const vector<string>& move);

public:
    virtual bool init(const vector<string>& move);

    void setOrgPosition(const Vec2& pos);

    /*获得角色移动的移动的比值*/
    Vec2 getAddPos();

    void setAttack(_joystick_attack* attack);

    void removeTouchCatched();

    void _update();

    bool isInside(const Vec2& pos);

private:
    _joystick_attack* attack = nullptr;
    Sprite* button_bk = nullptr;
    Sprite* button = nullptr;

private:
    Vec2 originalPos;
    Touch* catched = nullptr;
    Switch switch_isCatched;
};

class Joystick::_joystick_attack : public Node {
public:
    static _joystick_attack* create(const vector<string>& attack);

    virtual bool init(const vector<string>& attack);
    void setOrgPosition(const Vec2& pos);
    bool isInside(const Vec2& pos);
    bool isCannonCanRotate();

    Vec2 getAddPos();

    void removeTouchCatched();

    void _update();

private:
    Switch switch_buttonDisbale;
    Switch switch_canAttack;
    Switch switch_cannonRotate;  //炮管是否进行旋转
    Switch switch_isCatched;

private:
    Vec2 originalPos;
    Touch* catched = nullptr;
    Sprite* joystick_bk = nullptr;
    Sprite* joystick = nullptr;
    Sprite* disable = nullptr;
};

class Joystick::_joystick_skill : public Node {
public:
    static _joystick_skill* create(const vector<string>& skill);

    virtual bool init(const vector<string>& skill);
    virtual void setPosition(const Vec2& pos) override;

    // x:横坐标与横坐标比值 y:纵坐标与纵坐标比值
    Vec2 getAddPos();

    void removeTouchCatched();

    bool isInside(const Vec2& pos);
    void updateJoystick();
    void _update();

private:
    Menu* menu = nullptr;
    MenuItemSprite* button = nullptr;
    Label* timeShow = nullptr;
    Sprite* joystick_button = nullptr;
    Sprite* joystick_bk = nullptr;
    // Sprite* joystick_arrow = nullptr;
private:
    Vec2 originalPos;
    Switch switch_isCatched;
    Touch* catched = nullptr;
};

#endif