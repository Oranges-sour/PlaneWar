#include "header/BulletForEnemy1.h"

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/Random.h"

BulletForEnemy1* BulletForEnemy1::create(
    const string& bulletFrame,
    const vector<string>& particleOnExplode /*爆炸时的粒子特效*/,
    const vector<string>& particleOnMove /*移动时的粒子特效*/)
{
    BulletForEnemy1* bullet = new (std::nothrow) BulletForEnemy1();
    if (bullet && bullet->initWithSpriteFrameName(bulletFrame) &&
        bullet->init(particleOnExplode, particleOnMove)) {
        bullet->autorelease();
        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

bool BulletForEnemy1::init(const vector<string>& particleOnExplode,
                           const vector<string>& particleOnMove)
{
    this->b_particleOnExplode = particleOnExplode;
    this->b_particleOnMove = particleOnMove;
    this->setVisible(false);
    return true;
}

Vec2 BulletForEnemy1::shoot(const Vec2& from, float moveSpeed,
                            float particleLiveTime_OnMove,
                            float particleCreateSpeed_OnMove,
                            float particleLiveTime_OnExplode,
                            unsigned int particleQuantity_Onexplode,
                            float damage, float angle)
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
    auto ro = RotateBy::create(0.2f, 240.0f);
    this->runAction(RepeatForever::create(ro));
    //////////////////////////////////

    //////////////////////////////////
    //创建动作
    //防止k无限大
    if (MyMath::float_equal(heroPos.x, from.x)) {
        heroPos.x -= 0.5;
    }
    const float k1 = (heroPos.y - from.y) / (heroPos.x - from.x);
    const float a1 = DEG::atan(k1) + angle;
    float aX = moveSpeed * abs(DEG::cos(a1));
    float aY = moveSpeed * abs(DEG::sin(a1));
    if (heroPos.x < from.x) {
        aX = -aX;
    }
    if (heroPos.y < from.y) {
        aY = -aY;
    }
    addPos.x = aX;
    addPos.y = aY;
    //////////////////////////////////

    if (MyDefault.option.particleQuality != 0) {
        auto v = 0.0f;
        v = particleCreateSpeed_OnMove *
            (2.0f / MyDefault.option.particleQuality);
        this->schedule([&](float) { this->showParticleOnMove(); }, v,
                       "createParticle");
    }

    this->schedule([&](float) { this->_update(); }, "_update");

    return Vec2::ZERO;
}

void BulletForEnemy1::getAttack(float) { this->kill(); }

void BulletForEnemy1::kill()
{
    this->setVisible(false);
    this->showParticleOnExplode();
    this->removeFromParent();
}

void BulletForEnemy1::showParticleOnMove()
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

void BulletForEnemy1::showParticleOnExplode()
{
    float moveSpeed = 300;

    auto call = [&]() {
        rand_int r0(0, b_particleOnExplode.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            this->b_particleOnExplode[r0()], this->particleLiveTime_OnExplode);
        if (particle) {
            particle->setPosition(this->getPosition());
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

void BulletForEnemy1::onContact(Node* target)
{
    auto ob = dynamic_cast<basic_Object*>(target);
    if (ob && !this->isAttackFinish()) {
        ob->getAttack(this->damage);
        this->kill();
        this->attackFinish();
    }
}

void BulletForEnemy1::_update()
{
    float frameRate = Director::getInstance()->getFrameRate();
    this->setPosition(this->getPosition() + addPos / frameRate);
}
