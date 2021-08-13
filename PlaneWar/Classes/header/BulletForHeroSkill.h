#ifndef __BULLET_FOR_HERO_SKILL_H__
#define __BULLET_FOR_HERO_SKILL_H__

/*
废弃的
*/

#if 1

#include "Bullet.h"
class BulletForHeroSkill : public basic_Bullet
{

};

#else

#include "cocos2d.h"
USING_NS_CC;

#include "Bullet.h"
#include "Switch.h"

class basic_Object;

class BulletForHeroSkill : public basic_Bullet
{
public:
	static BulletForHeroSkill* create(const string& bullet,
		const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

	Vec2 shoot(const Vec2& from/*运动起始点*/,
		const Vec2& to/*出场时的运动终点*/,
		float particleLiveTime_OnMove/*粒子生存时间*/,
		float particleCreateSpeed_OnMove/*粒子生产速度 x秒1个*/,
		float particleLiveTime_OnExplode/*爆炸时的粒子生存时间*/,
		unsigned int particleQuantity_Onexplode/*爆炸时粒子粒子数量*/,
		float moveSpeed/*运动速度 像素/每秒*/,
		float range/*自动攻击时的侦测范围*/,
		float damage/*子弹伤害*/);

public://base_Object
	//virtual void getAttack(int)override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	virtual void onContact(Node* target)override;
	virtual void kill()override;
	virtual void _update()override;
private:

	//更新子弹旋转角度
	//void updateRotation();
private:
	//已经被攻击过的敌人
	vector<basic_Object*> haveBeenAttackedEnemy;

	//子弹上一帧时的坐标
	//Vec2 lastPosition = Vec2::ZERO;

	float particleLiveTime_OnMove = 0.0f;//粒子生存时间_移动中
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量

	//子弹击中目标时的伤害
	float damage = 0;

	//显示粒子
	void showParticleOnMove();
	void showParticleOnExplode();

	//子弹在移动和爆炸时的frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;

	float range = 0;
	float moveSpeed = 0;

	Switch switch_isFinishFirstMove;
	Switch switch_moveFinish;
};

#endif


#endif // !__BULLET_FOR_HERO_SKILL_H__
