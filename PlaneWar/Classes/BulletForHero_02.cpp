#include "header/BulletForHero_02.h"

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/Random.h"

BulletForHero_02* BulletForHero_02::create(
    const string& bulletFrame,
    const vector<string>& particleOnExplode /*爆炸时的粒子特效*/,
    const vector<string>& particleOnMove /*移动时的粒子特效*/)
{
    BulletForHero_02* bullet = new (std::nothrow) BulletForHero_02();
    if (bullet && bullet->initWithSpriteFrameName(bulletFrame) &&
        bullet->init(particleOnExplode, particleOnMove)) {
        bullet->autorelease();
        return bullet;
    }

    CC_SAFE_DELETE(bullet);
    return nullptr;
}

Vec2 BulletForHero_02::shoot(const Vec2& from, float moveSpeed,
                             float particleLiveTime_OnMove,
                             float particleCreateSpeed_OnMove,
                             float particleLiveTime_OnExplode,
                             unsigned int particleQuantity_Onexplode, int range,
                             float damage, float angle, float angleRotateSpeed)
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

    this->runAction(in);

    //返回值
    Vec2 return_result = Vec2::ZERO;

    rand_float r0(-10, 10);
    rand_float r1(-30, 30);
    this->rotateSpeed = angleRotateSpeed + r1();

    float newAngle = MyMath::addRotation(angle, r0());
    if (MyMath::float_equal(newAngle / 90, 0)) {
        newAngle += 1;
    }
    this->setRotation(newAngle);
    float k = DEG::tan(-newAngle);
    auto frameRate = Director::getInstance()->getFrameRate();

    float x = moveSpeed / frameRate / sqrtf(POT(k) + 1);
    if (newAngle > 90 && newAngle < 270) {
        x = -x;
    }
    float y = k * x;
    auto heroPos = GameManager::getInstance()->getHero()->getPosition();
    return_result = heroPos + Vec2(x, y);
    this->addPos = Vec2(x, y);
    //////////////////////////////////

    if (MyDefault.option.particleQuality != 0) {
        auto v = 0.0f;
        v = particleCreateSpeed_OnMove *
            (2.0f / MyDefault.option.particleQuality);
        this->schedule([&](float) { this->showParticleOnMove(); }, v,
                       "createParticle");
    }

    this->schedule([&](float) { this->_update(); }, "_update");

    this->scheduleOnce([&](float) { switch_canSearch.setSwitch(true); }, 0.1,
                       "delayToSerch");

    this->scheduleOnce([&](float) { switch_canSearch.setSwitch(false); },
                       0.1 + 1, "delayToStopSerch");

    return return_result;
}

bool BulletForHero_02::init(const vector<string>& particleOnExplode,
                            const vector<string>& particleOnMove)
{
    this->b_particleOnExplode = particleOnExplode;
    this->b_particleOnMove = particleOnMove;
    this->setVisible(false);
    return true;
}

void BulletForHero_02::_update()
{
    //子弹坐标
    const auto& thisPosition = this->getPosition();
    //获得离此子弹最近的敌人
    auto enemy = GameManager::getInstance()->getNearestEnemyWithLevel(
        thisPosition, range);
    if (enemy && switch_canSearch.isSet()) {
        //敌人坐标
        const auto& enemyPosition = enemy->getPosition();
        //子弹和敌人相差的角度
        const float rotationDifference =
            MyMath::getRotation(thisPosition, enemyPosition);

        //当前子弹的旋转角度
        const float thisRotation = this->getRotation();

        //帧率
        const float frameRate = Director::getInstance()->getFrameRate();

        //与敌人夹角大于一次旋转的最小角度,进行旋转
        if (abs(rotationDifference - thisRotation) > rotateSpeed / frameRate) {
            //判断度数是加(true)还是减(false)
            auto judgeAngle = [&]() -> bool {
                float temp = rotationDifference - thisRotation;
                if (temp > 180 || (temp < 0 && temp > -180)) {
                    return false;
                }
                return true;
            };

            //返回将正比例函数旋转一定角度之后的斜率(k)
            auto rotateK = [](float angle, float originalK) -> float {
                float a0 = DEG::atan(originalK);
                float a1 = a0 - angle;

                //角度为九十度的倍数,k容易无限大因此少量更改旋转的角度
                if (MyMath::float_equal(a1 / 90, 0)) {
                    a1 += 1;
                }
                return DEG::tan(a1);
            };

            //新的斜率
            float newK = 0;
            //新的旋转角度
            float newRotation = 0;
            {
                float angle = rotateSpeed / frameRate;
                float nowRotation = this->getRotation();
                float k = DEG::tan(-thisRotation);
                if (judgeAngle()) {
                    newK = rotateK(angle, k);
                    newRotation = MyMath::addRotation(nowRotation, angle);
                } else {
                    newK = rotateK(-angle, k);
                    newRotation = MyMath::addRotation(nowRotation, -angle);
                }
            }

            //移动的距离
            const float moveDistance = moveSpeed / frameRate;
            addPos.x = moveDistance / sqrt(POT(newK) + 1);
            if (newRotation > 90 && newRotation < 270) {
                //向左移动
                addPos.x = -addPos.x;
            }
            addPos.y = newK * addPos.x;

            this->setRotation(newRotation);
        }
    }
    this->setPosition(thisPosition + addPos);
}

void BulletForHero_02::kill()
{
    this->setVisible(false);
    this->showParticleOnExplode();
    this->removeFromParent();
}

void BulletForHero_02::onContact(Node* target)
{
    auto ob = dynamic_cast<basic_Object*>(target);
    if (ob && !this->isAttackFinish()) {
        ob->getAttack(this->damage);
        this->kill();
        this->attackFinish();
    }
}

void BulletForHero_02::showParticleOnMove()
{
    auto call = [&]() {
        rand_int r0(0, b_particleOnMove.size() - 1);
        //创建粒子,从不同形状中随机一个
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

void BulletForHero_02::showParticleOnExplode()
{
    auto e = Random::getEngine();
    float moveSpeed = 300.0f;
    // float frameRate = Director::getInstance()->getFrameRate();
    auto call = [&]() {
        rand_int r0(0, b_particleOnExplode.size() - 1);
        //创建粒子,从不同形状中随机一个
        auto particle = ParticleOnExplode::create(
            this->b_particleOnExplode[r0()], this->particleLiveTime_OnExplode);
        if (particle) {
            rand_float r0(-5, 5);
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

    //如果特效质量不是无特效
    if (MyDefault.option.particleQuality != 0) {
        auto v = static_cast<unsigned int>(MyMath::floatToInt(
            particleQuantity_OnExplode *
            static_cast<float>(MyDefault.option.particleQuality / 2.0f)));
        for (size_t x = 0; x < v; ++x) {
            call();
        }
    }
}
