#include "header/Enemy.h"

#include "header/MyMath.h"


void base_attackEnemy::shake(float time)
{
	shakeTime += time;

	switch_isOnShake.setSwitch(true);

	this->unschedule("_shakeTimeDown");
	this->schedule(
		[&](float)
		{
			if (shakeTime <= 0)
			{
				shakeTime = 0;
				switch_isOnShake.setSwitch(false);
				this->unschedule("_shakeTimeDown");
				return;
			}
			auto frame = Director::getInstance()->getFrameRate();
			shakeTime -= (1.0 / frame);
		}, "_shakeTimeDown");
}
