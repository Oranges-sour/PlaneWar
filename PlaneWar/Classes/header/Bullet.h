#ifndef __BULLET_H__
#define __BULLET_H__

#include <vector>
#include <string>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "Switch.h"
#include "basic_Object.h"

/*注意事项
	1.请给子弹留出三个zOrder空间,供爆炸,粒子和子弹本身使用

	zOrder+1    --爆炸粒子
	子弹zOrder  --子弹
	zOrder-1    --移动粒子
	*/

class basic_Bullet : public basic_Object//不要使用此类创建任何实例
{
protected:
	basic_Bullet() {}
	~basic_Bullet() {}
	//第一次完成攻击调用
	void attackFinish() { switch_attackFinish.setSwitch(true); }
	bool isAttackFinish() { return switch_attackFinish.isSet(); }
private:
	Switch switch_attackFinish;
};

#include "BulletForEnemy0.h"
#include "BulletForEnemy1.h"
#include "BulletForHero_01.h"
#include "BulletForHero_02.h"
#include "BulletForHeroSkill.h"


#endif