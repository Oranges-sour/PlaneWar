#ifndef __ENEMY_GROUP_H__
#define __ENEMY_GROUP_H__


#include "cocos2d.h"
USING_NS_CC;

//#include "Enemy.h"
#include "MyDefault.h"

class base_attackEnemy;

class EnemyGroup : public Sprite
{
public:
	static EnemyGroup* create(const string& frame);

	//������������֡
	void setLineFrame(const string& frame);
	//���÷�Χ��ʾ����֡
	void setCircleFrame(const string& frame);

	//��������С���ļ��
	void setLineIntervalDistance(float distance);

	//��ӵ���
	void addEnemyToGroup(base_attackEnemy* enemy);

	//���������Χ
	float getSerchRadius();

	//����������Χ
	void setSerchRadius(float radius);	

	//����Group, ��Ӣ�۽��뷶Χ�����(��һ��)
	void setActive();

	//ˢ������
	void refreshLine();

	//��Ϊ�뾶�ĸı�,��Ҫ����Բ
	void updateCircle();

	virtual void removeFromParent()override;

	virtual bool init()override;

	void _update();
	
private:
	float lineIntervalDistance;
	float serchRadius;
	string lineFrame;
	string circleFrame;
	Label* numberOfEnemy;
	vector<Sprite*> lineDots;
	vector<Sprite*> circle;
	vector<base_attackEnemy*> enemyPool;
};


#endif