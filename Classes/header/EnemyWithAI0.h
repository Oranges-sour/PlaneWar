#ifndef __ENEMY_WITH_AI0_H__
#define __ENEMY_WITH_AI0_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Enemy.h"

class EnemyAI0;
class HitPointBar_forEnemy;

class EnemyWithAI0 : public base_attackEnemy
{
public:
	static EnemyWithAI0* create(const string& enemy, const string& physicsInfo);

	void setEnemy(int hitPoint,
		vector<string>& particleOnExplode/*��ըʱ��������Ч*/,
		float particleLiveTime_OnExplode,
		unsigned int particleQuantity_OnExplode);

	void setWeaponAndAddChild(base_Weapon* weapon);

	void setExp(const map<ExpType, string>& expFrame,
		int hp, int ep, const string& physicsInfo);


public://����̳�	
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
private://�������
	void showParticleOnDead();
	vector<string> particleOnExplode/*��ըʱ��������Ч*/;
	float particleLiveTime_OnExplode = 0.0f;//��������ʱ��_��ը��
	unsigned int particleQuantity_OnExplode = 0;//��ըʱ����������	
private://����
	void createExp();
	map<ExpType, string> expFrames;
	string exp_physicsInfo;
	int exp_hp = 0;
	int exp_ep = 0;
private:
	base_Weapon* weapon = nullptr;
	HitPointBar_forEnemy* hitPointBar = nullptr;
	EnemyAI0* AI = nullptr;
};

#endif // !__ENEMY_WITH_AI0_H__
