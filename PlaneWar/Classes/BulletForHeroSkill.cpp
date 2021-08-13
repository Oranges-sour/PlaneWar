#include "header/BulletForHeroSkill.h"

/*
废弃的
*/

#if 1

#else

#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/MyDefault.h"

BulletForHeroSkill* BulletForHeroSkill::create(const string& bulletFrame,
	const vector<string>& particleOnExplode/*爆炸时的粒子特效*/,
	const vector<string>& particleOnMove/*移动时的粒子特效*/)
{
	BulletForHeroSkill* bullet = new (std::nothrow) BulletForHeroSkill();
	if (bullet && bullet->initWithSpriteFrameName(bulletFrame))
	{
		bullet->b_particleOnExplode = particleOnExplode;
		bullet->b_particleOnMove = particleOnMove;
		bullet->setVisible(false);

		auto os = GameManager::getInstance()->getOwnScheduler();
		auto oa = GameManager::getInstance()->getOwnActionManager();

		bullet->setScheduler(os);
		bullet->setActionManager(oa);

		bullet->autorelease();
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

Vec2 BulletForHeroSkill::shoot(const Vec2& from/*运动起始点*/,
	const Vec2& to/*出场时的运动终点*/,
	float particleLiveTime_OnMove/*粒子生存时间*/,
	float particleCreateSpeed_OnMove/*粒子生产速度 x秒1个*/,
	float particleLiveTime_OnExplode/*爆炸时的粒子生存时间*/,
	unsigned int particleQuantity_Onexplode/*爆炸时粒子粒子数量*/,
	float moveSpeed/*运动速度 像素/每秒*/,
	float range,
	float damage/*子弹伤害*/)
{
	this->range = range;
	this->moveSpeed = moveSpeed;
	this->damage = damage;
	this->particleLiveTime_OnMove = particleLiveTime_OnMove;
	this->particleQuantity_OnExplode = particleQuantity_Onexplode;
	this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
	this->setOpacity(0);
	this->setVisible(true);

	this->setPosition(from);

	auto rotate = RotateBy::create(0.2f, 90);
	this->runAction(RepeatForever::create(rotate));

	float time = MyMath::distance(from, to) / this->moveSpeed;
	auto moveTo = MoveTo::create(time, to);
	auto act = EaseInOut::create(moveTo, 2.0);
	//创建渐入
	auto in = FadeIn::create(0.02f);
	this->runAction(in);

	//////////////////////////////////

	auto callback = CallFunc::create(
		[&]()
		{
			switch_isFinishFirstMove.setSwitch(true);
			switch_moveFinish.setSwitch(true);
		});
	//创建序列
	auto line = Sequence::create(act, callback, nullptr);
	this->runAction(line);

	if (MyDefault.option.particleQuality != 0)
	{
		auto v = 0.0f;
		v = particleCreateSpeed_OnMove * (2.0f / MyDefault.option.particleQuality);
		this->schedule([&](float)
			{
				this->showParticleOnMove();
			}, v, "_showParticleOnMove");
	}
	

	this->schedule(
		[&](float)
		{
			this->_update();
		}, "_update");

	return Vec2::ZERO;
}

void BulletForHeroSkill::onContact(Node* target)
{
	const auto isCanAttack = [&](basic_Object* ob)->bool
	{
		for (auto& it : haveBeenAttackedEnemy)
		{
			//表示已经被攻击过
			if (ob == it)
			{
				return false;
			}
		}

		return true;
	};

	auto ob = dynamic_cast<basic_Object*>(target);
	if (ob && isCanAttack(ob))
	{
		ob->getAttack(this->damage);

		haveBeenAttackedEnemy.push_back(ob);
	}
}

void BulletForHeroSkill::kill()
{
	this->showParticleOnExplode();
	this->removeFromParent();
}

void BulletForHeroSkill::_update()
{
	if (switch_isFinishFirstMove.isSet() && switch_moveFinish.isSet())
	{
		//如果当前的动作执行一半的时候不会kill,只有这次完了才会
		auto hero = GameManager::getInstance()->getHero();
		auto heroSkill = hero->getSkill();
		if (!heroSkill->isRunSkill())
		{
			this->kill();
			return;
		}

		auto newAttack = [&](basic_Enemy* enemy)
		{
			//每次重新运动清除之前的记录
			haveBeenAttackedEnemy.clear();
			switch_moveFinish.setSwitch(false);

			auto getToPos = [&]()->Vec2
			{
				auto enemyPos = enemy->getPosition();
				auto thisPos = this->getPosition();

				float enemyDistance = MyMath::distance(enemyPos, thisPos);
				float moveDistance = enemyDistance + 200;

				return MyMath::randPos(MyMath::getPosOnLine(thisPos, enemyPos, moveDistance), 20, 60);

			};

			auto toPos = getToPos();

			float time = MyMath::distance(this->getPosition(), toPos) / this->moveSpeed;
			auto moveTo = MoveTo::create(time, toPos);
			auto act = EaseInOut::create(moveTo, 1.5);

			//////////////////////////////////

			auto callback = CallFunc::create(
				[&]()
				{
					switch_moveFinish.setSwitch(true);
				});

			//auto delay = DelayTime::create(0.02f);
			//创建序列
			auto line = Sequence::create(act, /*delay,*/ callback, nullptr);
			this->runAction(line);
		};

		//如果this周围有敌人,先攻击这个敌人.如果没有,选一个离hero最近的敌人
		auto enemy = GameManager::getInstance()->getNearestEnemyWithLevel(this->getPosition(), this->range);
		if (enemy)
		{
			newAttack(enemy);
		}
		else
		{
			auto enemy = GameManager::getInstance()->getNearestEnemyWithLevel(hero->getPosition(), this->range);
			if (enemy)
			{
				newAttack(enemy);
			}
		}

	}
}

void BulletForHeroSkill::showParticleOnMove()
{
	auto call = [&]()
	{
		auto& e = Random::getEngine();
		rand_int r0(0, b_particleOnMove.size() - 1);//范围可能改变,不能设置成 static
		auto particle = ParticleOnMove::create(this->b_particleOnMove[r0(e)], this->particleLiveTime_OnMove,1.5f);
		if (particle)
		{
			particle->setPosition(this->getPosition());
			GameManager::getInstance()->addParticleChild(particle);
		}
	};

	if (MyDefault.option.particleQuality != 0)
	{
		call();
	}
}

void BulletForHeroSkill::showParticleOnExplode()
{
	auto call = [&]()
	{
		auto& e = Random::getEngine();
		rand_int r0(0, b_particleOnExplode.size() - 1);//范围可能改变,不能设置成 static
		auto particle = ParticleOnExplode::create(this->b_particleOnExplode[r0(e)],
			this->particleLiveTime_OnExplode);
		if (particle)
		{
			particle->setPosition(this->getPosition());
			GameManager::getInstance()->addParticleChild(particle);
		}
	};

	if (MyDefault.option.particleQuality != 0)
	{
		auto v = static_cast<unsigned int>
			(MyMath::floatToInt(
				particleQuantity_OnExplode * static_cast<float>(MyDefault.option.particleQuality / 2.0f)
			));
		for (size_t x = 0; x < v; ++x)
		{
			call();
		}
	}
}

#endif