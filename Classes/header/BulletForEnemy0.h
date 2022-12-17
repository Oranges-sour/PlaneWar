#ifndef __BULLET_FOR_ENEMY00_H__
#define __BULLET_FOR_ENEMY00_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Bullet.h"


class BulletForEnemy0 : public basic_Bullet
{
public:
	static BulletForEnemy0* create(const string& bullet,
		const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

	Vec2 shoot(const Vec2& from/*运动起始点*/,
		float moveSpeed/*运动速度 像素/每秒*/,
		float particleLiveTime_OnMove/*粒子生存时间*/,
		float particleCreateSpeed_OnMove/*粒子生产速度 x秒1个*/,
		float particleLiveTime_OnExplode/*爆炸时的粒子生存时间*/,
		unsigned int particleQuantity_Onexplode/*爆炸时粒子粒子数量*/,
		float damage/*子弹伤害*/);

	virtual bool init(const vector<string>& particleOnExplode,
		const vector<string>& particleOnMove);

public://base_Object
	
	virtual void getAttack(float)override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	virtual void kill()override;
	virtual void onContact(Node* target)override;
	virtual void _update()override;
private:
	void showParticleOnMove();
	void showParticleOnExplode();

private:
	float particleLiveTime_OnMove = 0.0f;//粒子生存时间_移动中
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量

	//子弹击中目标时的伤害
	float damage = 0;

	//子弹在移动和爆炸时的frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;

	Vec2 addPos;

};

#endif

