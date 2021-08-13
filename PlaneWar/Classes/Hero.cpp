#include "header/Hero.h"

#include "header/Bullet.h"
#include "header/GameManager.h"
#include "header/HitPointBar.h"
#include "header/Joystick.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/PhysicsShapeCache.h"
#include "header/Weapon.h"
#include "header/XMLManager.h"

USING_NS_CC;
using namespace std;

Hero* Hero::create(const string& heroFrame, const string& cannonFrame)
{
    Hero* hero = new (std::nothrow) Hero();
    if (hero && hero->initWithSpriteFrameName(heroFrame) && hero->init()) {
        hero->cannon = Sprite::createWithSpriteFrameName(cannonFrame);
        hero->autorelease();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

void Hero::removeFromParent() { basic_Object::removeFromParent(); }

void Hero::kill() { this->removeFromParent(); }

void Hero::shootBullet()
{
    if (this->weapon) {
        Vec2 p1 = this->weapon->attack(this->getPosition());
        if (p1 != Vec2::ZERO) {
            float r = MyMath::getRotation(this->getPosition(), p1);
            this->cannon->setRotation(r);
            // this->lastRotation = r;
        }
    }
}

void Hero::setSkill(HeroSkill_base* skill)
{
    if (this->skill) {
        if (this->skill->isRunSkill()) {
            this->skill->stopSkill();
        }
        this->skill->removeFromParent();
        this->skill = nullptr;
    }

    this->skill = skill;
    this->addChild(skill);
}

HeroSkill_base* Hero::getSkill() { return this->skill; }

float Hero::getEnergyPoint() { return ep_now; }

void Hero::setEnergyPoint(float p)
{
    if (p < 0)
        ep_now = 0;
    else if (p > ep_max)
        ep_now = ep_max;
    else
        ep_now = p;
}

float Hero::getMaxEnergyPoint() { return ep_max; }

void Hero::setMaxEnergyPoint(float p) { ep_max = p; }

float Hero::getHitPoint() { return hp_now; }

void Hero::setHitPoint(float p)
{
    if (p < 0)
        hp_now = 0;
    else if (p > hp_max)
        hp_now = hp_max;
    else
        hp_now = p;
}

float Hero::getMaxHitPoint() { return hp_max; }

void Hero::setMaxHitPoint(float p) { hp_max = p; }

void Hero::getAttack(float damage)
{
    if (skill->isRunSkill()) {
        if (skill->getSkillType() == HeroSkillType::shield) {
            return;
        }
    }
    rand_int r0(1, 100);
    //”–º∏¬ √‚…À
    if (r0() <= MyDefault.updateInfo.defendSubDamage.nowValue) {
        auto showText = Hero_showText::create();
        GameManager::getInstance()->addOthersChild(showText);
    } else {
        this->setHitPoint(this->getHitPoint() - damage);
    }
}

bool Hero::init()
{
    this->schedule([&](float) { _update(); }, "_update");
    return true;
}

void Hero::setMoveSpeed(float v) { this->moveSpeed = v; }

float Hero::getMoveSpeed() { return this->moveSpeed; }

Sprite* Hero::getCannon() { return this->cannon; }

void Hero::_update() { this->cannon->setPosition(this->getPosition()); }

void Hero::setWeaponAndAddChild(base_Weapon* weapon)
{
    if (weapon) {
        this->weapon = weapon;
        this->addChild(weapon);
    }
}

float Hero::getAttackInterval() { return this->weapon->getAttackInterval(); }

//////////////////////////////////////////////////////////////////////////

Hero_showText* Hero_showText::create()
{
    auto t = new (std::nothrow) Hero_showText();
    if (t && t->init()) {
        t->autorelease();
        return t;
    }
    CC_SAFE_DELETE(t);
    return nullptr;
}

bool Hero_showText::init()
{
    label =
        Label::createWithTTF("", MyDefault.font.medium, 35, Size(250, 0),
                             TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setColor(Color3B(220, 220, 220));
    label->setString(XMLManager::getString("sub_damage"));
    label->setOpacity(0);
    this->addChild(label);

    auto act0 = FadeTo::create(0.1f, 255 * 0.8f);
    auto act1 = FadeTo::create(0.02f, 255 * 0.2f);
    auto act2 = FadeTo::create(0.06f, 255 * 1.0f);
    auto act3 = DelayTime::create(0.15f);
    auto act4 = FadeTo::create(0.1f, 0);
    auto s = Sequence::create(act0, act1, act2, act3, act4, nullptr);
    label->runAction(s);

    auto hero = GameManager::getInstance()->getHero();
    auto size = max(hero->getContentSize().width / 2,
                    hero->getContentSize().height / 2);
    auto heroPos = hero->getPosition();
    auto labelPos = MyMath::randPos(heroPos, size * 0.8f, size * 1.2f);
    label->setPosition(labelPos);

    this->scheduleOnce([&](float) { this->removeFromParent(); }, 0.5f,
                       "__autoKill");

    return true;
}
