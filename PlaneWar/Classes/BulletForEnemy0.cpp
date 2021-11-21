#include "header/BulletForEnemy0.h"

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/Random.h"

BulletForEnemy0* BulletForEnemy0::create(
    const string& bulletFrame,
    const vector<string>& particleOnExplode /*爆炸时的粒子特效*/,
    const vector<string>& particleOnMove /*移动时的粒子特效*/)
{
    BulletForEnemy0* bullet = new (std::nothrow) BulletForEnemy0();
    if (bullet && bullet->initWithSpriteFrameName(bulletFrame) &&
        bullet->init(particleOnExplode, particleOnMove)) {
        bullet->autorelease();
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

Vec2 BulletForEnemy0::shoot(const Vec2& from, float moveSpeed,
                            float particleLiveTime_OnMove,
                            float particleCreateSpeed_OnMove,
                            float particleLiveTime_OnExplode,
                            unsigned int particleQuantity_Onexplode,
                            float damage)
{
    //初始化
    this->damage = damage;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleQuantity_OnExplode = particleQuantity_Onexplode;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->setPosition(from);
    this->setVisible(true);

    Vec2 heroPos = GameManager::getInstance()->getHero()->getPosition();
    //////////////////////////////////
    //设置旋转角度
    const float r = MyMath::getRotation(from, heroPos);
    this->setRotation(r);
    //////////////////////////////////

    //////////////////////////////////
    //创建动作
    this->addPos = MyMath::getPosOnLine(Vec2::ZERO, heroPos - from, moveSpeed);
    //////////////////////////////////

    if (MyDefault.option.particleQuality != 0) {
        const float v = particleCreateSpeed_OnMove *
                        (2.0f / MyDefault.option.particleQuality);
        this->schedule([&](float) { this->showParticleOnMove(); }, v,
                       "__createParticle");
    }

    this->schedule([&](float) { this->_update(); }, "_update");
    return Vec2::ZERO;
}

bool BulletForEnemy0::init(const vector<string>& particleOnExplode,
                           const vector<string>& particleOnMove)
{
    this->b_particleOnExplode = particleOnExplode;
    this->b_particleOnMove = particleOnMove;
    this->setVisible(false);
    return true;
}

void BulletForEnemy0::getAttack(float) { this->kill(); }

void BulletForEnemy0::kill()
{
    this->setVisible(false);
    this->showParticleOnExplode();
    this->removeFromParent();
}

void BulletForEnemy0::showParticleOnMove()
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

void BulletForEnemy0::showParticleOnExplode()
{
    float moveSpeed = 300;
    auto call = [&]() {
        rand_int r0(0, b_particleOnExplode.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            this->b_particleOnExplode[r0()], this->particleLiveTime_OnExplode);
        if (particle) {
            auto e = Random::getEngine();
            rand_float r0(-10, 10);
            particle->setPosition(this->getPosition() + Vec2(r0(), r0()));
            GameManager::getInstance()->addParticleChild(particle);

            auto thisPos = this->getPosition();
            float k = addPos.y / addPos.x;
            rand_float r1(-45, 45);
            float angle = DEG::atan(k);
            float newAngle = angle + r1();
            float newK = DEG::tan(newAngle);
            float x = moveSpeed / sqrt(POT(newK) + 1);
            if (addPos.x < 0) {
                x = -x;
            }
            float y = x * newK;
            particle->setAddPos(Vec2(x, y));
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

void BulletForEnemy0::onContact(Node* target)
{
    auto ob = dynamic_cast<basic_Object*>(target);
    if (ob && !this->isAttackFinish()) {
        ob->getAttack(this->damage);
        this->kill();
        this->attackFinish();
    }
}

void BulletForEnemy0::_update()
{
    const float frameRate = Director::getInstance()->getFrameRate();
    this->setPosition(this->getPosition() + addPos / frameRate);
}
