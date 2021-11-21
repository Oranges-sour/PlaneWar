#include "header/BulletForHero_01.h"

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/Random.h"

BulletForHero_01* BulletForHero_01::create(
    const string& bulletFrame,
    const vector<string>& particleOnExplode /*爆炸时的粒子特效*/,
    const vector<string>& particleOnMove /*移动时的粒子特效*/)
{
    BulletForHero_01* bullet = new (std::nothrow) BulletForHero_01();
    if (bullet && bullet->initWithSpriteFrameName(bulletFrame) &&
        bullet->init(particleOnExplode, particleOnMove)) {
        bullet->autorelease();
        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

Vec2 BulletForHero_01::shoot(const Vec2& from, float moveSpeed,
                             float particleLiveTime_OnMove,
                             float particleCreateSpeed_OnMove,
                             float particleLiveTime_OnExplode,
                             unsigned int particleQuantity_Onexplode, int range,
                             float damage)
{
    this->range = range;
    this->moveSpeed = moveSpeed;
    this->damage = damage;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleQuantity_OnExplode = particleQuantity_Onexplode;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->setPosition(from);
    this->setOpacity(0);
    this->setVisible(true);

    //创建渐入
    auto in = FadeIn::create(0.01f);

    //创建移动
    ccBezierConfig bezier;

    //返回值
    Vec2 return_result = Vec2::ZERO;

    auto enemy =
        GameManager::getInstance()->getNearestEnemyWithLevel(from, range);
    //如果范围内有敌人
    if (enemy) {
        bezier = MyMath::getBezier(
            from, MyMath::randPos(enemy->getPosition(), 20.0f, 40.0f));
        return_result = enemy->getPosition();
    } else {
        Vec2 p1 = Vec2(
            from.x, Director::getInstance()->getVisibleSize().height + 25.0f);
        bezier = MyMath::getBezier(from, p1);
        return_result = p1;
    }
    this->lastBezier = bezier;
    float time = MyMath::getBezierLenth(from, bezier) / this->moveSpeed;
    auto moveTo = BezierTo::create(time, bezier);

    //渐入和移动一起执行
    auto together = Spawn::create(in, moveTo, nullptr);

    //////////////////////////////////

    auto callback = CallFunc::create([&]() { this->moveFinish(); });
    //创建序列
    auto line = Sequence::create(together, callback, nullptr);
    this->runAction(line);

    if (MyDefault.option.particleQuality != 0) {
        auto v = 0.0f;
        v = particleCreateSpeed_OnMove *
            (2.0f / MyDefault.option.particleQuality);
        this->schedule([&](float) { this->showParticleOnMove(); }, v,
                       "createParticle");
    }

    this->schedule([&](float) { this->_update(); }, "_update");

    return return_result;
}

bool BulletForHero_01::init(const vector<string>& particleOnExplode,
                            const vector<string>& particleOnMove)
{
    this->b_particleOnExplode = particleOnExplode;
    this->b_particleOnMove = particleOnMove;
    this->setVisible(false);

    return true;
}

void BulletForHero_01::_update() { this->updateRotation(); }

void BulletForHero_01::moveFinish()
{
    if (!checkForNewTaget()) {
        this->stopAllActions();
        //创建贝塞尔曲线
        ccBezierConfig bezier;
        rand_float r0(-100.0f, 100.0f);

        //计算贝塞尔
        bezier = MyMath::getBezier(
            this->getPosition(),
            Vec2(this->getPosition().x + r0(), this->getPosition().y + 150.0f),
            lastBezier);
        //运动时间
        float time = MyMath::getBezierLenth(this->getPosition(), bezier) /
                     (this->moveSpeed / 2);
        //创建动作
        auto moveTo = BezierTo::create(time, bezier);

        this->runAction(moveTo);
        this->schedule(
            [&](float) {
                if (checkForNewTaget()) {
                    //停止倒计时爆炸和搜索新目标
                    this->unschedule("__killBullet_timer");
                    this->unschedule("__checkForNewTaget_timer");
                }
            },
            "__checkForNewTaget_timer");

        this->scheduleOnce([&](float) { this->kill(); }, time - 0.1f,
                           "__killBullet_timer");
    }
}

void BulletForHero_01::updateRotation()
{
    if (!switchForRotation.isSet()) {
        lastPosition = this->getPosition();
        switchForRotation.setSwitch(true);
    } else {
        float rotation_now =
            MyMath::getRotation(lastPosition, this->getPosition());
        this->setRotation(rotation_now);
        lastPosition = this->getPosition();
    }
}

void BulletForHero_01::kill()
{
    this->setVisible(false);
    this->showParticleOnExplode();
    this->removeFromParent();
}

void BulletForHero_01::onContact(Node* target)
{
    auto ob = dynamic_cast<basic_Object*>(target);
    if (ob && !this->isAttackFinish()) {
        ob->getAttack(this->damage);
        this->kill();
        this->attackFinish();
    }
}

bool BulletForHero_01::checkForNewTaget()
{
    auto enemy = GameManager::getInstance()->getNearestEnemyWithLevel(
        this->getPosition(), range / 3);
    if (enemy) {
        this->stopAllActions();
        ccBezierConfig bezier;

        Vec2 from = this->getPosition();
        bezier = MyMath::getBezier(
            from, MyMath::randPos(enemy->getPosition(), 20.0f, 40.0f),
            lastBezier);

        this->lastBezier = bezier;
        float time = MyMath::getBezierLenth(from, bezier) / this->moveSpeed;
        auto moveTo = BezierTo::create(time, bezier);

        auto callback = CallFunc::create([&]() { this->moveFinish(); });
        //创建序列
        auto line = Sequence::create(moveTo, callback, nullptr);
        this->runAction(line);

        return true;
    }
    return false;
}

void BulletForHero_01::showParticleOnMove()
{
    auto call = [&]() {
        rand_int r0(
            0, b_particleOnMove.size() - 1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnMove::create(this->b_particleOnMove[r0()],
                                               this->particleLiveTime_OnMove);
        if (particle) {
            particle->setPosition(this->getPosition());
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        call();
    }
}

void BulletForHero_01::showParticleOnExplode()
{
    auto call = [&]() {
        rand_int r0(0, b_particleOnExplode.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            this->b_particleOnExplode[r0()], this->particleLiveTime_OnExplode);
        if (particle) {
            particle->setPosition(this->getPosition());
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        auto v = static_cast<unsigned int>(MyMath::floatToInt(
            particleQuantity_OnExplode *
            static_cast<float>(MyDefault.option.particleQuality / 2.0f)));
        for (size_t x = 0; x < v; ++x) {
            call();
        }
    }
}

Vec2 BulletForHero_01::getLastPosition() { return lastPosition; }

void BulletForHero_01::setLastPosition(const Vec2& pos) { lastPosition = pos; }
