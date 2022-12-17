#ifndef __BULLET_FOR_HERO_SKILL_H__
#define __BULLET_FOR_HERO_SKILL_H__

/*
������
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
		const vector<string>& particleOnExplode/*��ըʱ��������Ч*/,
		const vector<string>& particleOnMove/*�ƶ�ʱ��������Ч*/);

	Vec2 shoot(const Vec2& from/*�˶���ʼ��*/,
		const Vec2& to/*����ʱ���˶��յ�*/,
		float particleLiveTime_OnMove/*��������ʱ��*/,
		float particleCreateSpeed_OnMove/*���������ٶ� x��1��*/,
		float particleLiveTime_OnExplode/*��ըʱ����������ʱ��*/,
		unsigned int particleQuantity_Onexplode/*��ըʱ������������*/,
		float moveSpeed/*�˶��ٶ� ����/ÿ��*/,
		float range/*�Զ�����ʱ����ⷶΧ*/,
		float damage/*�ӵ��˺�*/);

public://base_Object
	//virtual void getAttack(int)override;
	//virtual Vec2 getLastPosition()override;
	//virtual void setLastPosition(const Vec2& pos)override;
	virtual void onContact(Node* target)override;
	virtual void kill()override;
	virtual void _update()override;
private:

	//�����ӵ���ת�Ƕ�
	//void updateRotation();
private:
	//�Ѿ����������ĵ���
	vector<basic_Object*> haveBeenAttackedEnemy;

	//�ӵ���һ֡ʱ������
	//Vec2 lastPosition = Vec2::ZERO;

	float particleLiveTime_OnMove = 0.0f;//��������ʱ��_�ƶ���
	float particleLiveTime_OnExplode = 0.0f;//��������ʱ��_��ը��
	unsigned int particleQuantity_OnExplode = 0;//��ըʱ����������

	//�ӵ�����Ŀ��ʱ���˺�
	float damage = 0;

	//��ʾ����
	void showParticleOnMove();
	void showParticleOnExplode();

	//�ӵ����ƶ��ͱ�ըʱ��frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;

	float range = 0;
	float moveSpeed = 0;

	Switch switch_isFinishFirstMove;
	Switch switch_moveFinish;
};

#endif


#endif // !__BULLET_FOR_HERO_SKILL_H__
