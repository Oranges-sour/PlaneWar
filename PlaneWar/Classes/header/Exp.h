#ifndef __ENEMY_EXP_H__
#define __ENEMY_EXP_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Enemy.h"

class Exp : public basic_Enemy
{
public:
	static Exp* create(const  string& filename, ExpType type, const  string& physicsInfo);
	void setExpValue(float v);
	void startExplode(const Vec2& pos);//开始刚创建时的爆炸移动
public:
	virtual void removeFromParent()override;
	virtual EnemyType getEnemyType()override;
	virtual bool init()override;
public://base_object
	//virtual Vec2 getLastPosition();
	//virtual void setLastPosition(const Vec2& pos);
	//virtual void getAttack(int damage);
	virtual void kill()override;
	virtual void onContact(Node* target)override;
	virtual void _update()override;
private:
	Switch switch_explodeFinish;//是否完成出场爆炸
	Switch switch_startMoveToHero;//第一次运动时设为true,此时不进行范围检测

	float value = 0;
	float moveSpeed;
	float maxMoveSpeed;
	ExpType expType;
};




#endif // !__ENEMY_WITH_AI0_H__
