#include "header/Particle.h"

#include "header/GameManager.h"
#include "header/Random.h"

//////////////////////////////////
ParticleOnMove* ParticleOnMove::create(const string& parFrame, float liveTime,
                                       float rate)
{
    ParticleOnMove* par = new (std::nothrow) ParticleOnMove();
    if (par && par->initWithSpriteFrameName(parFrame)) {
        par->autorelease();
        par->initParticle(liveTime, rate);

        return par;
    }
    CC_SAFE_DELETE(par);
    return nullptr;
}

void ParticleOnMove::updateParticle()
{
    auto frameRate = Director::getInstance()->getFrameRate();
    this->setRotation(this->getRotation() + addRotate / frameRate);
    this->setPosition(this->getPosition() + Vec2(addX, addY) / frameRate);
}

void ParticleOnMove::setAddPos(const Vec2& addPos)
{
    addX = addPos.x;
    addY = addPos.y;
}

bool ParticleOnMove::initParticle(float liveTime, float rate)
{
    rand_float r0(0, 3);
    rand_int r1(5, 10);
    rand_float r2(1.5, 30);
    rand_bool r3;

    this->scheduleOnce(schedule_selector(ParticleOnMove::KillParticle),
                       liveTime);

    auto act = ScaleTo::create(r0() / 100.0f + 0.05f, 1.0f);
    this->runAction(act);

    this->addRotate = r1() * 60;

    constexpr float RANGE = 20.0f;
    addX = r2() / RANGE;
    addY = r2() / RANGE;
    if (r3()) {
        addX = -addX;
    }
    if (r3()) {
        addY = -addY;
    }
    addX *= (rate * 60);
    addY *= (rate * 60);

    return true;
}

void ParticleOnMove::KillParticle(float)
{
    rand_float r(0, 5);
    float time = r() / 30.0f + 0.1f;
    auto act0 = ScaleTo::create(time, 0.0f);
    auto act1 = FadeOut::create(time + 0.2f);
    auto runAct = Spawn::create(act0, act1, nullptr);
    auto callback = CallFunc::create([&]() { this->removeFromParent(); });
    auto line = Sequence::create(runAct, callback, nullptr);
    this->runAction(line);
}
//////////////////////////////////

//////////////////////////////////
ParticleOnExplode* ParticleOnExplode::create(const string& parFrame,
                                             float liveTime, float rate)
{
    ParticleOnExplode* par = new (std::nothrow) ParticleOnExplode();
    if (par && par->initWithSpriteFrameName(parFrame)) {
        par->autorelease();

        par->initParticle(liveTime, rate);

        return par;
    }
    CC_SAFE_DELETE(par);
    return nullptr;
}

bool ParticleOnExplode::initParticle(float liveTime, float rate)
{
    rand_float r0(0, 3);
    rand_int r1(5, 10);
    rand_float r2(1.5, 25);
    rand_bool r3;

    this->scheduleOnce(schedule_selector(ParticleOnMove::KillParticle),
                       liveTime);

    auto act = ScaleTo::create(r0() / 100.0f + 0.05f, 1.0f);
    this->runAction(act);

    this->addRotate = r1() * 60;

    constexpr float RANGE = 10.0f;
    addX = r2() / RANGE;
    addY = r2() / RANGE;
    if (r3()) {
        addX = -addX;
    }
    if (r3()) {
        addY = -addY;
    }
    addX *= (rate * 60);
    addY *= (rate * 60);

    return true;
}

void ParticleOnExplode::updateParticle()
{
    auto frameRate = Director::getInstance()->getFrameRate();
    this->setRotation(this->getRotation() + addRotate / frameRate);
    this->setPosition(this->getPosition() + Vec2(addX, addY) / frameRate);
}
