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
	void startExplode(const Vec2& pos);//��ʼ�մ���ʱ�ı�ը�ƶ�
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
	Switch switch_explodeFinish;//�Ƿ���ɳ�����ը
	Switch switch_startMoveToHero;//��һ���˶�ʱ��Ϊtrue,��ʱ�����з�Χ���

	float value = 0;
	float moveSpeed;
	float maxMoveSpeed;
	ExpType expType;
};




#endif // !__ENEMY_WITH_AI0_H__
