#ifndef __BULLET_FOR_HERO01_H__
#define __BULLET_FOR_HERO01_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Bullet.h"

class BulletForHero_01 : public basic_Bullet
{
public:
	static BulletForHero_01* create(const string& bullet,
		const vector<string>& particleOnExplode/*��ըʱ��������Ч*/,
		const vector<string>& particleOnMove/*�ƶ�ʱ��������Ч*/);

	Vec2 shoot(const Vec2& from/*�˶���ʼ��*/,
		float moveSpeed/*�˶��ٶ� ����/ÿ��*/,
		float particleLiveTime_OnMove/*��������ʱ��*/,
		float particleCreateSpeed_OnMove/*���������ٶ� x��1��*/,
		float particleLiveTime_OnExplode/*��ըʱ����������ʱ��*/,
		unsigned int particleQuantity_Onexplode/*��ըʱ������������*/,
		int range/*׷�ٷ�Χ*/,
		float damage/*�ӵ��˺�*/);

	virtual bool init(const vector<string>& particleOnExplode/*��ըʱ��������Ч*/,
		const vector<string>& particleOnMove/*�ƶ�ʱ��������Ч*/);

public://base_Object
	//virtual void getAttack(int)override;
	virtual Vec2 getLastPosition()override;
	virtual void setLastPosition(const Vec2& pos)override;
	virtual void kill()override;
	virtual void onContact(Node* target)override;
	virtual void _update()override;
private:
	void moveFinish();
	//Ѱ����Ŀ��
	bool checkForNewTaget();

	//��ʾ����
	void showParticleOnMove();
	void showParticleOnExplode();

	//�����ӵ���ת�Ƕ�
	void updateRotation();
private:
	//��һ���ӵ�·��
	ccBezierConfig lastBezier;

	//�ӵ���һ֡ʱ������
	Vec2 lastPosition = Vec2::ZERO;

	float particleLiveTime_OnMove = 0.0f;//��������ʱ��_�ƶ���
	float particleLiveTime_OnExplode = 0.0f;//��������ʱ��_��ը��
	unsigned int particleQuantity_OnExplode = 0;//��ըʱ����������

	//�ӵ�����Ŀ��ʱ���˺�
	float damage = 0;

	//�ӵ��ܽ���׷�ٵķ�Χ
	int range = 0;

	//�ƶ��ٶ�  ����/ÿ��
	float moveSpeed = 0.0f;

	//�ӵ����ƶ��ͱ�ըʱ��frame
	vector<string> b_particleOnMove;
	vector<string> b_particleOnExplode;

	//��void updateRotation();ʹ�õĿ��ر���
	Switch switchForRotation;
};

#endif // !__BULLET_FOR_HERO01_H__
