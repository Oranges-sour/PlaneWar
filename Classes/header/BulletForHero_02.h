#ifndef __BULLET_FOR_HERO02_H__
#define __BULLET_FOR_HERO02_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Bullet.h"

class BulletForHero_02 : public basic_Bullet
{
public:
	static BulletForHero_02* create(const string& bullet,
		const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

	Vec2 shoot(const Vec2& from/*运动起始点*/,
		float moveSpeed/*运动速度 像素/每秒*/,
		float particleLiveTime_OnMove/*粒子生存时间*/,
		float particleCreateSpeed_OnMove/*粒子生产速度 x秒1个*/,
		float particleLiveTime_OnExplode/*爆炸时的粒子生存时间*/,
		unsigned int particleQuantity_Onexplode/*爆炸时粒子粒子数量*/,
		int range/*追踪范围*/,
		float damage/*子弹伤害*/,
		float angle/*子弹初始朝向*/,
		float angleRotateSpeed/*旋转速度(度/秒)*/);

	virtual bool init(const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

public://base_Object
	//virtual void getAttack(int)override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	virtual void kill()override;
	virtual void onContact(Node* target)override;
	virtual void _update()override;
private:

	//显示粒子
	void showParticleOnMove();
	void showParticleOnExplode();

private:

	float particleLiveTime_OnMove = 0.0f;//粒子生存时间_移动中
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量

	//子弹击中目标时的伤害
	float damage = 0;

	//子弹能进行追踪的范围
	int range = 0;

	//移动速度  像素/每秒
	float moveSpeed = 0;

	float rotateSpeed = 0;

	Vec2 addPos;

	Switch switch_canSearch;
	//子弹在移动和爆炸时的frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;
};

#endif // !__BULLET_FOR_HERO01_H__
