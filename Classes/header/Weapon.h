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
        const string& bullet,                     //�ӵ�����֡
        const vector<string>& particleOnExplode,  //��ը����
        const vector<string>& particleOnMove,     //�ƶ�����
        const string& physicsInfo,
        float moveSpeed,                //�ƶ��ٶ�,ÿ��
        float particleLiveTime_OnMove,  //�ƶ���������ʱ��(С��Χ���)
        float particleCreateSpeed_OnMove,  //�ƶ����Ӳ����ٶ�,ÿ��
        float particleLiveTime_OnExplode,  //��ը��������ʱ��(С��Χ���)
        unsigned int particleQuantity_OnExplode,  //��ը��������
        float damage,                             //�˺�
        int energy,                               //��������
        int range,                                //׷�ٷ�Χ
        float attackInterval                      //�������
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
        const string& bullet,                     //�ӵ�����֡
        const vector<string>& particleOnExplode,  //��ը����
        const vector<string>& particleOnMove,     //�ƶ�����
        const string& physicsInfo,
        float moveSpeed,                //�ƶ��ٶ�,ÿ��
        float particleLiveTime_OnMove,  //�ƶ���������ʱ��(С��Χ���)
        float particleCreateSpeed_OnMove,  //�ƶ����Ӳ����ٶ�,ÿ��
        float particleLiveTime_OnExplode,  //��ը��������ʱ��(С��Χ���)
        unsigned int particleQuantity_OnExplode,  //��ը��������
        float damage,                             //�˺�
        int energy,                               //��������
        int range,                                //׷�ٷ�Χ
        float angleRotateSpeed,                   /*��ת�ٶ�(��/��)*/
        float attackInterval                      //�������
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
        const string& bullet,                     //�ӵ�����֡
        const vector<string>& particleOnExplode,  //��ը����
        const vector<string>& particleOnMove,     //�ƶ�����
        const string& physicsInfo,
        float moveSpeed,                //�ƶ��ٶ�,ÿ��
        float particleLiveTime_OnMove,  //�ƶ���������ʱ��(С��Χ���)
        float particleCreateSpeed_OnMove,  //�ƶ����Ӳ����ٶ�,ÿ��
        float particleLiveTime_OnExplode,  //��ը��������ʱ��(С��Χ���)
        unsigned int particleQuantity_OnExplode,  //��ը��������
        float damage                              //�˺�
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
        const string& bullet,                     //�ӵ�����֡
        const vector<string>& particleOnExplode,  //��ը����
        const vector<string>& particleOnMove,     //�ƶ�����
        const string& physicsInfo,
        float moveSpeed,                //�ƶ��ٶ�,ÿ��
        float particleLiveTime_OnMove,  //�ƶ���������ʱ��(С��Χ���)
        float particleCreateSpeed_OnMove,  //�ƶ����Ӳ����ٶ�,ÿ��
        float particleLiveTime_OnExplode,  //��ը��������ʱ��(С��Χ���)
        unsigned int particleQuantity_OnExplode,  //��ը��������
        float damage                              //�˺�
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

//������
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
        const string& bullet,                     //�ӵ�����֡
        const vector<string>& particleOnExplode,  //��ը����
        const vector<string>& particleOnMove,     //�ƶ�����
        const string& physicsInfo,
        float moveSpeed,                //�ƶ��ٶ�,ÿ��
        float particleLiveTime_OnMove,  //�ƶ���������ʱ��(С��Χ���)
        float particleCreateSpeed_OnMove,  //�ƶ����Ӳ����ٶ�,ÿ��
        float particleLiveTime_OnExplode,  //��ը��������ʱ��(С��Χ���)
        unsigned int particleQuantity_OnExplode,  //��ը��������
        int range,                                //׷�ٷ�Χ
        float damage                              //�˺�
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
