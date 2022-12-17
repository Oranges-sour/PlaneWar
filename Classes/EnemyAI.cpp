#include "header/EnemyAI.h"

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyMath.h"
#include "header/Random.h"

constexpr int aiActionTag = 1000;

EnemyAI0* EnemyAI0::create(EnemyWithAI0* ene)
{
    auto AI = new (std::nothrow) EnemyAI0();
    if (AI) {
        AI->switch_attackIsFinish.setSwitch(true);
        AI->enemy = ene;
        return AI;
    }
    CC_SAFE_DELETE(AI);
    return nullptr;
}

void EnemyAI0::runAI()
{
    if (enemy->isOnShake()) {
        enemy->stopActionByTag(aiActionTag);
        switch_attackIsFinish.setSwitch(true);
        this->unschedule("_shoot");
        return;
    }
    if (switch_attackIsFinish.isSet()) {
        switch_attackIsFinish.setSwitch(false);
        static constexpr float RANGE_MAX = 300.0f;
        static constexpr float RANGE_MIN = 80.0f;

        auto heroPos = GameManager::getInstance()->getHero()->getPosition();
        auto bezier =
            MyMath::getBezier(enemy->getPosition(),
                              MyMath::randPos(heroPos, RANGE_MIN, RANGE_MAX));
        if (enemy->getPosition().x < bezier.endPosition.x) {
            enemy->setRotation(enemy->getRotation() + 20);
        } else {
            enemy->setRotation(enemy->getRotation() - 20);
        }
        auto act = BezierTo::create(1.5f, bezier);
        auto func = CallFunc::create([&]() {
            switch_attackIsFinish.setSwitch(true);

            enemy->setRotation(0);
        });
        auto func1 = CallFunc::create([&]() { enemy->setRotation(0); });
        auto delay = DelayTime::create(0.8f);
        auto s = Sequence::create(act, func1, delay, func, nullptr);
        s->setTag(aiActionTag);
        enemy->runAction(s);
        this->schedule([&](float) { this->shootBullet(NULL); }, 0.3f, 4, 0.1f,
                       "_shoot");
    }
}

void EnemyAI0::shootBullet(float) { enemy->attack(); }

EnemyAI1* EnemyAI1::create(EnemyWithAI1* ene)
{
    auto AI = new (std::nothrow) EnemyAI1();
    if (AI) {
        AI->switch_attackIsFinish.setSwitch(true);
        AI->enemy = ene;

        AI->autorelease();
        return AI;
    }
    CC_SAFE_DELETE(AI);
    return nullptr;
}

void EnemyAI1::runAI()
{
    if (enemy->isOnShake()) {
        enemy->stopActionByTag(aiActionTag);
        switch_attackIsFinish.setSwitch(true);
        // this->unschedule("_shoot");
        return;
    }
    if (switch_attackIsFinish.isSet()) {
        switch_attackIsFinish.setSwitch(false);

        static constexpr float RANGE_MAX = 300.0f;
        static constexpr float RANGE_MIN = 80.0f;

        auto heroPos = GameManager::getInstance()->getHero()->getPosition();
        auto bezier =
            MyMath::getBezier(enemy->getPosition(),
                              MyMath::randPos(heroPos, RANGE_MIN, RANGE_MAX));
        if (enemy->getPosition().x < bezier.endPosition.x) {
            enemy->setRotation(enemy->getRotation() + 20);
        } else {
            enemy->setRotation(enemy->getRotation() - 20);
        }
        auto move = BezierTo::create(1.5f, bezier);
        auto func =
            CallFunc::create([&]() { switch_attackIsFinish.setSwitch(true); });
        auto doAttack = CallFunc::create([&]() {
            for (int x = 0; x < 3; ++x) {  //移动结束放子弹
                enemy->attack();
            }

            enemy->setRotation(0);
        });
        auto delay = DelayTime::create(0.8f);
        auto s = Sequence::create(move, doAttack, delay, func, nullptr);
        s->setTag(aiActionTag);
        enemy->runAction(s);

        //移动开始时射出一个子弹
        enemy->attack();
    }
}
