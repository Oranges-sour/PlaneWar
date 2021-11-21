#include "header/EnemyWithAI1.h"

#include <array>
using namespace std;

#include "header/Bullet.h"
#include "header/EnemyAI.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/HitPointBar.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/PhysicsShapeCache.h"
#include "header/Random.h"

//////////////////////////////////////////////////////////////////////////

EnemyWithAI1* EnemyWithAI1::create(const string& enemy,
                                   const string& physicsInfo)
{
    auto _enemy = new (std::nothrow) EnemyWithAI1();
    if (_enemy && _enemy->initWithSpriteFrameName(enemy)) {
        _enemy->AI = EnemyAI1::create(_enemy);

        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, _enemy);
        _enemy->setTag(tag_enemy);
        _enemy->addChild(_enemy->AI);
        _enemy->autorelease();

        return _enemy;
    }
    CC_SAFE_DELETE(_enemy);
    return nullptr;
}

void EnemyWithAI1::setEnemy(int hitPoint, vector<string>& particleOnExplode,
                            float particleLiveTime_OnExplode,
                            unsigned int particleQuantity_OnExplode)
{
    this->schedule([&](float) { this->_update(); }, "_update");

    this->particleOnExplode = particleOnExplode;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->setHitPoint(hitPoint);
    this->setFullHitPoint(hitPoint);

    this->hitPointBar = HitPointBar_forEnemy::create(
        this, Imgs.gamePage0.vp_enemyHitPointBar[0],
        Imgs.gamePage0.vp_enemyHitPointBar[1]);
    hitPointBar->setOpacity(0);
    GameManager::getInstance()->addOthersChild(hitPointBar);
}

void EnemyWithAI1::setExp(const map<ExpType, string>& expFrame, int hp, int ep,
                          const string& physicsInfo)
{
    this->expFrames = expFrame;
    this->exp_hp = hp;
    this->exp_ep = ep;
    this->exp_physicsInfo = physicsInfo;
}

void EnemyWithAI1::setWeapon(Weapon_Enemy01* weapon)
{
    if (weapon) {
        this->addChild(weapon);
        this->weapon = weapon;
    }
}

void EnemyWithAI1::attack()
{
    auto e = EnemyWithAI1_child::create(Imgs.onGame.enemy1Child.p_enemy,
                                        MyDefault.enemy02_chlid.physicsInfo);
    e->setEnemy(Imgs.onGame.enemy1Child.vp_explode,
                MyDefault.enemy02_chlid.particleLiveTime_OnExplode,
                MyDefault.enemy02_chlid.particleQuantity_OnExplode);
    e->setWeaponAndAddChild(weapon->clone());
    e->setPosition(this->getPosition());
    GameManager::getInstance()->addEnemyChild(e);
}

EnemyType EnemyWithAI1::getEnemyType() { return EnemyType::attack; }

EnemySerchLevel EnemyWithAI1::getEnemySerchLevel()
{
    return EnemySerchLevel::mid;
}

void EnemyWithAI1::removeFromParent()
{
    this->hitPointBar->removeFromParent();
    basic_Enemy::removeFromParent();
}

void EnemyWithAI1::pause()
{
    AI->pause();
    basic_Enemy::pause();
}

void EnemyWithAI1::resume()
{
    AI->resume();
    basic_Enemy::resume();
}

void EnemyWithAI1::kill()
{
    GameManager::getInstance()->enemyDie();
    this->setVisible(false);
    this->showParticleOnDead();
    this->createExp();
    this->removeFromParent();
}

void EnemyWithAI1::getAttack(float damage)
{
    if (this->isOnAttack()) {
        if (MyDefault.option.particleQuality != 0) {
            auto p = GLProgramCache::getInstance()->getGLProgram(
                MyDefault.myShader.light);
            auto ps = GLProgramState::create(p);
            ps->setUniformFloat("light", 1.4f);
            this->setGLProgram(p);
            this->setGLProgramState(ps);

            this->unschedule("_glProgramBack");
            this->scheduleOnce(
                [&](float) {
                    auto p = GLProgramCache::getInstance()->getGLProgram(
                        MyDefault.myShader.defaultShader);
                    this->setGLProgram(p);
                },
                0.02f, "_glProgramBack");
        }

        this->hitPointBar->fadeInBar();

        this->unschedule("_showHitPointBar");
        this->scheduleOnce([&](float) { this->hitPointBar->fadeOutBar(); },
                           1.5f, "_showHitPointBar");

        this->setHitPoint(this->getHitPoint() - damage);
    } else {
        this->stopAllActions();
        this->setVisible(true);
        auto act = Blink::create(0.1f, 1);
        this->runAction(act);
    }
}

void EnemyWithAI1::_update()
{
    if (this->isOnAttack()) {
        AI->runAI();
    }
    this->hitPointBar->setPosition(
        Vec2(this->getPosition().x,
             this->getPosition().y - this->getContentSize().height / 2 - 15));
    //必须最后执行
    this->checkAlive();
}

void EnemyWithAI1::checkAlive()
{
    if (this->getHitPoint() <= 0) {
        kill();
    }
}

void EnemyWithAI1::showParticleOnDead()
{
    auto call = [&]() {
        rand_int r0(
            0, particleOnExplode.size() - 1);  //范围可能改变,不能设置成 static
        auto particle =
            ParticleOnExplode::create(this->particleOnExplode[r0()],
                                      this->particleLiveTime_OnExplode, 1.5f);
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

void EnemyWithAI1::createExp()
{
    rand_int r0(1, 2);
    if (exp_ep != 0) {
        int num = r0();
        float ep = (float)exp_ep / num;
        for (int x = 0; x < num; ++x) {
            auto epExp =
                Exp::create(this->expFrames.find(ExpType::EnergyPoint)->second,
                            ExpType::EnergyPoint, exp_physicsInfo);
            epExp->setExpValue(ep);
            epExp->setPosition(this->getPosition());
            epExp->startExplode(this->getPosition());
            GameManager::getInstance()->addEnemyChild(epExp);
        }
    }
    if (exp_hp != 0) {
        int num = r0();
        float hp = (float)exp_hp / num;
        for (int x = 0; x < num; ++x) {
            auto hpExp =
                Exp::create(this->expFrames.find(ExpType::HitPoint)->second,
                            ExpType::HitPoint, exp_physicsInfo);
            hpExp->setExpValue(hp);
            hpExp->setPosition(this->getPosition());
            hpExp->startExplode(this->getPosition());
            GameManager::getInstance()->addEnemyChild(hpExp);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

EnemyWithAI1_child* EnemyWithAI1_child::create(const string& enemy,
                                               const string& physicsInfo)
{
    auto _enemy = new (std::nothrow) EnemyWithAI1_child();
    if (_enemy && _enemy->initWithSpriteFrameName(enemy)) {

        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, _enemy);
        _enemy->setTag(tag_enemy);
        _enemy->autorelease();

        return _enemy;
    }
    CC_SAFE_DELETE(_enemy);
    return nullptr;
}

void EnemyWithAI1_child::setEnemy(vector<string>& particleOnExplode,
                                  float particleLiveTime_OnExplode,
                                  unsigned int particleQuantity_OnExplode)
{
    this->schedule([&](float) { this->_update(); }, "_update");

    this->particleOnExplode = particleOnExplode;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;

    auto tempAdd = MyMath::randPos(Vec2::ZERO, 80.0f, 120.0f);
    auto move = MoveBy::create(1.0f, tempAdd);
    this->runAction(RepeatForever::create(move));

    auto p = GLProgramCache::getInstance()->getGLProgram("light");
    this->setGLProgram(p);

    auto ps = GLProgramState::create(p);
    ps->setUniformFloat("light", light);
    this->setGLProgramState(ps);

    float deadTime = 1.5f;
    this->scheduleOnce(
        [&](float) {
            this->attackOnKill();
            this->kill();
        },
        deadTime, "countDown_kill");

    this->schedule(
        [&](float) {
            auto frameRate = Director::getInstance()->getFrameRate();
            this->light += 6.0f / frameRate;
            auto ps = this->getGLProgramState();
            ps->setUniformFloat("light", light);
        },
        0, CC_REPEAT_FOREVER, 1.0f, "lightUp");

    auto scale = ScaleTo::create(0.5f, 1.5f);
    auto delay = DelayTime::create(1.0f);
    auto s = Sequence::create(delay, scale, nullptr);

    this->runAction(s);
}

void EnemyWithAI1_child::setWeaponAndAddChild(Weapon_Enemy01* weapon)
{
    if (weapon) {
        this->addChild(weapon);
        this->weapon = weapon;
    }
}

EnemyType EnemyWithAI1_child::getEnemyType() { return EnemyType::canSerch; }

EnemySerchLevel EnemyWithAI1_child::getEnemySerchLevel()
{
    return EnemySerchLevel::low;
}

void EnemyWithAI1_child::removeFromParent() { basic_Enemy::removeFromParent(); }

void EnemyWithAI1_child::kill()
{
    this->setVisible(false);
    this->showParticleOnDead();
    this->removeFromParent();
}

void EnemyWithAI1_child::getAttack(float damage)
{
    switch_resdyToKill.setSwitch(true);
}

void EnemyWithAI1_child::_update()
{
    if (switch_resdyToKill.isSet()) {
        this->kill();
    }
}

void EnemyWithAI1_child::attackOnKill()
{
    weapon->attack(this->getPosition(), -15);
    weapon->attack(this->getPosition(), 0);
    weapon->attack(this->getPosition(), 15);
}

void EnemyWithAI1_child::showParticleOnDead()
{
    auto call = [&]() {
        rand_int r0(
            0, particleOnExplode.size() - 1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            this->particleOnExplode[r0()], this->particleLiveTime_OnExplode);
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
