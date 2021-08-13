#ifndef __BULLET_FOR_HERO01_H__
#define __BULLET_FOR_HERO01_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Bullet.h"

class BulletForHero_01 : public basic_Bullet
{
public:
	static BulletForHero_01* create(const string& bullet,
		const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

	Vec2 shoot(const Vec2& from/*运动起始点*/,
		float moveSpeed/*运动速度 像素/每秒*/,
		float particleLiveTime_OnMove/*粒子生存时间*/,
		float particleCreateSpeed_OnMove/*粒子生产速度 x秒1个*/,
		float particleLiveTime_OnExplode/*爆炸时的粒子生存时间*/,
		unsigned int particleQuantity_Onexplode/*爆炸时粒子粒子数量*/,
		int range/*追踪范围*/,
		float damage/*子弹伤害*/);

	virtual bool init(const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
		const vector<string>& particleOnMove/*移动时的粒子特效*/);

public://base_Object
	//virtual void getAttack(int)override;
	virtual Vec2 getLastPosition()override;
	virtual void setLastPosition(const Vec2& pos)override;
	virtual void kill()override;
	virtual void onContact(Node* target)override;
	virtual void _update()override;
private:
	void moveFinish();
	//寻找新目标
	bool checkForNewTaget();

	//显示粒子
	void showParticleOnMove();
	void showParticleOnExplode();

	//更新子弹旋转角度
	void updateRotation();
private:
	//上一个子弹路径
	ccBezierConfig lastBezier;

	//子弹上一帧时的坐标
	Vec2 lastPosition = Vec2::ZERO;

	float particleLiveTime_OnMove = 0.0f;//粒子生存时间_移动中
	float particleLiveTime_OnExplode = 0.0f;//粒子生存时间_爆炸中
	unsigned int particleQuantity_OnExplode = 0;//爆炸时的粒子数量

	//子弹击中目标时的伤害
	float damage = 0;

	//子弹能进行追踪的范围
	int range = 0;

	//移动速度  像素/每秒
	float moveSpeed = 0.0f;

	//子弹在移动和爆炸时的frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;

	//供void updateRotation();使用的开关变量
	Switch switchForRotation;
};

#endif // !__BULLET_FOR_HERO01_H__
