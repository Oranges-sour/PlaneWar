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

	//设置线条精灵帧
	void setLineFrame(const string& frame);
	//设置范围显示精灵帧
	void setCircleFrame(const string& frame);

	//设置线条小点点的间隔
	void setLineIntervalDistance(float distance);

	//添加敌人
	void addEnemyToGroup(base_attackEnemy* enemy);

	//获得搜索范围
	float getSerchRadius();

	//设置搜索范围
	void setSerchRadius(float radius);	

	//激活Group, 在英雄进入范围后调用(仅一次)
	void setActive();

	//刷新线条
	void refreshLine();

	//因为半径的改变,需要更新圆
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