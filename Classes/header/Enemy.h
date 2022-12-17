#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <vector>
#include <random>
#include <map>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "Switch.h"
#include "basic_Object.h"

class base_EnemyAI;
class base_Weapon;

enum class EnemyType
{
	base,
	attack,//��attack��ζ��һ�����Ա�serch
	canSerch,
	cannotSerch,//����������
};

enum class EnemySerchLevel
{
	unknow,
	low,
	mid,
	high
};

enum class ExpType
{
	HitPoint,
	EnergyPoint
};

/*
ע��
һ��Enemyֻ�ܶ�Ӧһ��EnemyAI

*/

class basic_Enemy : public basic_Object//��Ҫʹ�ô��ഴ���κ�ʵ��
{
public:
	virtual EnemyType getEnemyType() { return EnemyType::base; }
	virtual EnemySerchLevel getEnemySerchLevel() { return EnemySerchLevel::unknow; }
protected:
	basic_Enemy() {}
	~basic_Enemy() {}
};

class base_attackEnemy : public basic_Enemy
{
public:
	virtual void setSerchRadius(float r) { serchRadius = r; }
	//���̽�ⷶΧ
	virtual float getSerchRadius() { return serchRadius; }
	//�Ƿ��ڹ���״̬(�������ڷ����ӵ�������)
	virtual bool isOnAttack() { return switch_isOnAttack.isSet(); }
	//����һ�ι���
	virtual void attack() = 0;
	//��ʼ����
	virtual void startAttack() { switch_isOnAttack.setSwitch(true); }
	//�Ƿ��ڵ�������
	virtual bool isInEnemyGroup() { return switch_isInEnemyGroup.isSet(); }
	//��ӵ�����ʱ����(��Ҫ�ֶ�����)
	virtual void setIsInEnemyGroup(bool state) { switch_isInEnemyGroup.setSwitch(state); }

	virtual float getHitPoint() { return nowHitPoint; }

	virtual float getFullHitPoint() { return fullHitPoint; }

	virtual void setFullHitPoint(float hitPoint) { fullHitPoint = hitPoint; }

	virtual void setHitPoint(float hitPoint) { nowHitPoint = hitPoint; }

	virtual void shake(float time);
	
	virtual bool isOnShake() { return switch_isOnShake.isSet(); }
protected:
	Switch switch_isOnAttack;
	Switch switch_isOnShake;
private:
	float nowHitPoint = 0;
	float fullHitPoint = 0;

	Switch switch_isInEnemyGroup;
	float serchRadius = 0;
	float shakeTime = 0;
};

#include "EnemyWithAI0.h"
#include "EnemyWithAI1.h"
#include "Exp.h"

#endif
