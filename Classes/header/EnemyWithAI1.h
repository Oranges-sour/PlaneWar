#ifndef __ENEMY_WITH_AI1_H__
#define __ENEMY_WITH_AI1_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Enemy.h"
#include "Weapon.h"
#include "Switch.h"

class EnemyAI1;
class HitPointBar_forEnemy;


class EnemyWithAI1 : public base_attackEnemy
{
public:
	static EnemyWithAI1* create(const string& enemy, const string& physicsInfo);

	void setEnemy(int hitPoint,
		vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		float particleLiveTime_OnExplode,
		unsigned int particleQuantity_OnExplode);

	void setExp(const map<ExpType, string>& expFrame,
		int hp, int ep, const string& physicsInfo);

	void setWeapon(Weapon_Enemy01* weapon);
public://父类继承	
	virtual void attack()override;
	virtual EnemyType getEnemyType()override;
	virtual EnemySerchLevel getEnemySerchLevel()override;
	virtual void removeFromParent()override;
	virtual void pause()override;
	virtual void resume()override;
public://base_Object
	virtual void kill()override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	//virtual void onContact(Node* target)override;
	virtual void getAttack(float damage)override;
	virtual void _update()override;
private:
	void checkAlive();
private://粒子相关
	void showParticleOnDead();
	vector<string> particleOnExplode/*爆炸时的粒子特效*/;
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量
private:
	void createExp();
	map<ExpType, string> expFrames;
	string exp_physicsInfo;
	int exp_hp = 0;
	int exp_ep = 0;
private:
	HitPointBar_forEnemy* hitPointBar = nullptr;
	EnemyAI1* AI = nullptr;
	Weapon_Enemy01* weapon = nullptr;
};

class EnemyWithAI1_child : public basic_Enemy
{
public:
	static EnemyWithAI1_child* create(const string& enemy, const string& physicsInfo);

	void setEnemy(vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		float particleLiveTime_OnExplode,
		unsigned int particleQuantity_OnExplode);

	void setWeaponAndAddChild(Weapon_Enemy01* weapon);


public://父类继承	
	virtual EnemyType getEnemyType()override;
	virtual EnemySerchLevel getEnemySerchLevel()override;
	virtual void removeFromParent()override;
public://base_Object
	virtual void kill()override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	//virtual void onContact(Node* target)override;
	virtual void getAttack(float damage)override;
	virtual void _update()override;
private:
	void attackOnKill();
private://粒子相关
	void showParticleOnDead();
	vector<string> particleOnExplode/*爆炸时的粒子特效*/;
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量	
private:
	Weapon_Enemy01* weapon = nullptr;
	Switch switch_resdyToKill;
	float light = 1.0f;//爆炸变亮
};

#endif // !__ENEMY_WITH_AI1_H__
