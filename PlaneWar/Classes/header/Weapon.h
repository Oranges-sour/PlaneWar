#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <string>
#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

class Hero;

class base_Weapon : public Node {
public:
    virtual Vec2 attack(const Vec2& pos) = 0;
    virtual base_Weapon* clone() = 0;
    virtual float getAttackInterval() = 0;
};

class Weapon_Hero01 : public base_Weapon {
public:
    static Weapon_Hero01* create();
    virtual Vec2 attack(const Vec2& from) override;
    virtual Weapon_Hero01* clone() override;

public:
    void setBulletInfo(
        const string& bullet,                     //子弹精灵帧
        const vector<string>& particleOnExplode,  //爆炸粒子
        const vector<string>& particleOnMove,     //移动粒子
        const string& physicsInfo,
        float moveSpeed,                //移动速度,每秒
        float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
        float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
        float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
        unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
        float damage,                             //伤害
        int energy,                               //能量消耗
        int range,                                //追踪范围
        float attackInterval                      //攻击间隔
    );

    virtual float getAttackInterval() override;

private:
    string bulletFrame;
    vector<string> particleOnExplode;
    vector<string> particleOnMove;
    string physicsInfo;
    float moveSpeed;
    float particleLiveTime_OnMove;
    float particleCreateSpeed_OnMove;
    float particleLiveTime_OnExplode;
    unsigned int particleQuantity_OnExplode;
    int range;
    int energy;
    float damage;
    float attackInterval;
};

class Weapon_Hero02 : public base_Weapon {
public:
    static Weapon_Hero02* create();
    virtual Vec2 attack(const Vec2& from) override;
    virtual Weapon_Hero02* clone() override;

public:
    void setBulletInfo(
        const string& bullet,                     //子弹精灵帧
        const vector<string>& particleOnExplode,  //爆炸粒子
        const vector<string>& particleOnMove,     //移动粒子
        const string& physicsInfo,
        float moveSpeed,                //移动速度,每秒
        float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
        float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
        float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
        unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
        float damage,                             //伤害
        int energy,                               //能量消耗
        int range,                                //追踪范围
        float angleRotateSpeed,                   /*旋转速度(度/秒)*/
        float attackInterval                      //攻击间隔
    );

    virtual float getAttackInterval() override;

private:
    string bulletFrame;
    vector<string> particleOnExplode;
    vector<string> particleOnMove;
    string physicsInfo;
    float moveSpeed;
    float particleLiveTime_OnMove;
    float particleCreateSpeed_OnMove;
    float particleLiveTime_OnExplode;
    unsigned int particleQuantity_OnExplode;
    int range;
    int energy;
    float damage;
    float angleRotateSpeed;
    float attackInterval;
};

class Weapon_Enemy00 : public base_Weapon {
public:
    static Weapon_Enemy00* create();
    virtual Vec2 attack(const Vec2& from) override;
    virtual bool init() override;
    virtual Weapon_Enemy00* clone() override;

public:
    void setBulletInfo(
        const string& bullet,                     //子弹精灵帧
        const vector<string>& particleOnExplode,  //爆炸粒子
        const vector<string>& particleOnMove,     //移动粒子
        const string& physicsInfo,
        float moveSpeed,                //移动速度,每秒
        float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
        float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
        float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
        unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
        float damage                              //伤害
    );

    virtual float getAttackInterval() override;

protected:
    string bulletFrame;
    vector<string> particleOnExplode;
    vector<string> particleOnMove;
    string physicsInfo;
    float moveSpeed;
    float particleLiveTime_OnMove;
    float particleCreateSpeed_OnMove;
    float particleLiveTime_OnExplode;
    unsigned int particleQuantity_OnExplode;
    float damage;
};

class Weapon_Enemy01 : public base_Weapon {
public:
    static Weapon_Enemy01* create();
    virtual Vec2 attack(const Vec2& from, float angle);
    virtual Vec2 attack(const Vec2& pos) { return Vec2::ZERO; }

    virtual bool init() override;
    virtual Weapon_Enemy01* clone() override;

public:
    void setBulletInfo(
        const string& bullet,                     //子弹精灵帧
        const vector<string>& particleOnExplode,  //爆炸粒子
        const vector<string>& particleOnMove,     //移动粒子
        const string& physicsInfo,
        float moveSpeed,                //移动速度,每秒
        float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
        float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
        float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
        unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
        float damage                              //伤害
    );

    virtual float getAttackInterval() override;

protected:
    string bulletFrame;
    vector<string> particleOnExplode;
    vector<string> particleOnMove;
    string physicsInfo;
    float moveSpeed;
    float particleLiveTime_OnMove;
    float particleCreateSpeed_OnMove;
    float particleLiveTime_OnExplode;
    unsigned int particleQuantity_OnExplode;
    float damage;
};

#if 1

//废弃的
class Weapon_HeroSkill : public base_Weapon {
};

#else

class Weapon_HeroSkill : public base_Weapon {
public:
    static Weapon_HeroSkill* create();
    virtual Vec2 attack(const Vec2&) override;

    virtual bool init() override;
    virtual Weapon_HeroSkill* clone() override;

public:
    void setBulletInfo(
        const string& bullet,                     //子弹精灵帧
        const vector<string>& particleOnExplode,  //爆炸粒子
        const vector<string>& particleOnMove,     //移动粒子
        const string& physicsInfo,
        float moveSpeed,                //移动速度,每秒
        float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
        float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
        float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
        unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
        int range,                                //追踪范围
        float damage                              //伤害
    );

    virtual float getAttackInterval() override;

protected:
    string bulletFrame;
    vector<string> particleOnExplode;
    vector<string> particleOnMove;
    string physicsInfo;
    float moveSpeed;
    float particleLiveTime_OnMove;
    float particleCreateSpeed_OnMove;
    float particleLiveTime_OnExplode;
    unsigned int particleQuantity_OnExplode;
    int range;
    float damage;
};
#endif

#endif
