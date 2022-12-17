#include "header/Exp.h"

#include "header/Bullet.h"
#include "header/EnemyAI.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/PhysicsShapeCache.h"
#include "header/Random.h"

Exp* Exp::create(const string& filename, ExpType type,
                 const string& physicsInfo)
{
    auto _ep = new (std::nothrow) Exp();
    if (_ep && _ep->initWithSpriteFrameName(filename)) {
        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, _ep);
        _ep->expType = type;
        _ep->setTag(tag_exp);

        _ep->init();

        _ep->autorelease();
        return _ep;
    }
    CC_SAFE_DELETE(_ep);
    return nullptr;
}

void Exp::setExpValue(float v) { this->value = v; }

void Exp::startExplode(const Vec2& pos)
{
    constexpr float RANGE_MAX = 150.0f;
    constexpr float RANGE_MIN = 60.0f;

    Vec2 to(MyMath::randPos(this->getPosition(), RANGE_MIN, RANGE_MAX));
    rand_float r1(0.3f, 0.5f);
    auto move = MoveTo::create(r1(), to);
    auto act = EaseOut::create(move, 1.8f);
    auto func =
        CallFunc::create([&]() { this->switch_explodeFinish.setSwitch(true); });
    auto s = Sequence::create(act, func, nullptr);
    this->runAction(s);
}

void Exp::onContact(Node* target)
{
    if (switch_explodeFinish.isSet()) {
        if (GameManager::getInstance()->isHeroDead()) {
            kill();
            return;
        }

        auto ob = dynamic_cast<Hero*>(target);
        switch (expType) {
            case ExpType::EnergyPoint: {
                ob->setEnergyPoint(ob->getEnergyPoint() + value);
                break;
            }
            case ExpType::HitPoint: {
                ob->setHitPoint(ob->getHitPoint() + value);
                break;
            }
            default:
                break;
        }
        kill();
    }
}

void Exp::removeFromParent() { basic_Enemy::removeFromParent(); }

EnemyType Exp::getEnemyType() { return EnemyType::cannotSerch; }

bool Exp::init()
{
    this->schedule([&](float) { this->_update(); }, "_update");

    return true;
}

void Exp::_update()
{
    if (GameManager::getInstance()->isHeroDead()) {
        return;
    }

    if (switch_explodeFinish.isSet()) {
        auto heroPos = GameManager::getInstance()->getHero()->getPosition();
        auto distance = MyMath::distance(this->getPosition(), heroPos);

        if (!switch_startMoveToHero.isSet()) {
            if (distance < 450) {
                switch_startMoveToHero.setSwitch(true);
                this->maxMoveSpeed = 1200;

                //速度渐渐增加
                this->schedule(
                    [&](float) {
                        if (moveSpeed < maxMoveSpeed) {
                            auto frameRate =
                                Director::getInstance()->getFrameRate();
                            moveSpeed += 1800 / frameRate;
                        } else {
                            this->unschedule("__addMoveSpeed");
                        }
                    },
                    "__addMoveSpeed");
            }
        } else {
            auto frameRate = Director::getInstance()->getFrameRate();
            auto deltaPos = MyMath::getPosOnLine(
                Vec2::ZERO, heroPos - getPosition(), moveSpeed);

            this->setPosition(this->getPosition() + deltaPos / frameRate);
            this->setRotation(this->getRotation() + moveSpeed / 50);
            if (distance < 250) {
                this->setScale(distance / 250);
            }
            if (distance < 200) {
                this->setOpacity((distance / 200) * 255);
            }
        }
    }
}

void Exp::kill() { removeFromParent(); }
