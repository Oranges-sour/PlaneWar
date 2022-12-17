#ifndef __ENEMY_AI_H__
#define __ENEMY_AI_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Switch.h"

class basic_Enemy;
class base_attackEnemy;

class EnemyWithAI0;
class EnemyWithAI1;


class base_EnemyAI : public Node
{
public:
	virtual void runAI() = 0;
};

class EnemyAI0 : public base_EnemyAI
{
public:
	static EnemyAI0* create(EnemyWithAI0* ene);
public:
	virtual void runAI()override;

	void shootBullet(float);
private:
	Switch switch_attackIsFinish;
private:
	EnemyWithAI0* enemy = nullptr;
};

class EnemyAI1 : public base_EnemyAI
{
public:
	static EnemyAI1* create(EnemyWithAI1* ene);
public:
	virtual void runAI()override;

private:
	Switch switch_attackIsFinish;
private:
	EnemyWithAI1* enemy = nullptr;
};

#endif
