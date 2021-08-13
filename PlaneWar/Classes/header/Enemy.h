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
	attack,//能attack意味着一定可以被serch
	canSerch,
	cannotSerch,//不会搜索到
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
注意
一种Enemy只能对应一种EnemyAI

*/

class basic_Enemy : public basic_Object//不要使用此类创建任何实例
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
	//获得探测范围
	virtual float getSerchRadius() { return serchRadius; }
	//是否处于攻击状态(不代表在发射子弹或其他)
	virtual bool isOnAttack() { return switch_isOnAttack.isSet(); }
	//进行一次攻击
	virtual void attack() = 0;
	//开始攻击
	virtual void startAttack() { switch_isOnAttack.setSwitch(true); }
	//是否在敌人组中
	virtual bool isInEnemyGroup() { return switch_isInEnemyGroup.isSet(); }
	//添加到组中时调用(不要手动调用)
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
