#include "header/EnemyWithAI0.h"

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

EnemyWithAI0* EnemyWithAI0::create(const string& enemy,
                                   const string& physicsInfo)
{
    auto _enemy = new (std::nothrow) EnemyWithAI0();
    if (_enemy && _enemy->initWithSpriteFrameName(enemy)) {
        _enemy->AI = EnemyAI0::create(_enemy);

        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, _enemy);
        _enemy->setTag(tag_enemy);
        _enemy->addChild(_enemy->AI);

        _enemy->autorelease();
        return _enemy;
    }
    CC_SAFE_DELETE(_enemy);
    return nullptr;
}

void EnemyWithAI0::setEnemy(int hitPoint, vector<string>& particleOnExplode,
                            float particleLiveTime_OnExplode,
                            unsigned int particleQuantity_OnExplode)
{
    this->schedule([&](float) { this->_update(); }, "_update");

    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->particleOnExplode = particleOnExplode;
    this->setFullHitPoint(hitPoint);
    this->setHitPoint(hitPoint);

    this->hitPointBar = HitPointBar_forEnemy::create(
        this, Imgs.gamePage0.vp_enemyHitPointBar[0],
        Imgs.gamePage0.vp_enemyHitPointBar[1]);
    hitPointBar->setOpacity(0);
    GameManager::getInstance()->addOthersChild(hitPointBar);
}

void EnemyWithAI0::setWeaponAndAddChild(base_Weapon* weapon)
{
    if (weapon) {
        this->addChild(weapon);
        this->weapon = weapon;
    }
}

void EnemyWithAI0::setExp(const map<ExpType, string>& expFrame, int hp, int ep,
                          const string& physicsInfo)
{
    this->exp_ep = ep;
    this->exp_hp = hp;
    this->expFrames = expFrame;
    this->exp_physicsInfo = physicsInfo;
}

void EnemyWithAI0::getAttack(float damage)
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

void EnemyWithAI0::kill()
{
    GameManager::getInstance()->enemyDie();
    this->setVisible(false);
    this->showParticleOnDead();
    this->createExp();
    this->removeFromParent();
}

void EnemyWithAI0::attack()
{
    if (weapon) {
        this->weapon->attack(this->getPosition());
    }
}

EnemyType EnemyWithAI0::getEnemyType() { return EnemyType::attack; }

EnemySerchLevel EnemyWithAI0::getEnemySerchLevel()
{
    return EnemySerchLevel::mid;
}

void EnemyWithAI0::removeFromParent()
{
    this->hitPointBar->removeFromParent();
    basic_Enemy::removeFromParent();
}

void EnemyWithAI0::pause()
{
    AI->pause();
    basic_Enemy::pause();
}

void EnemyWithAI0::resume()
{
    AI->resume();
    basic_Enemy::resume();
}

void EnemyWithAI0::_update()
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

void EnemyWithAI0::checkAlive()
{
    if (this->getHitPoint() <= 0) {
        kill();
    }
}

void EnemyWithAI0::showParticleOnDead()
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

void EnemyWithAI0::createExp()
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
