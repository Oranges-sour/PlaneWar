#include "header/Joystick.h"

#include <cstdlib>

#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyMath.h"
#include "header/ObjectPool.h"
#include "header/Particle.h"

////////////////////////////////////////////////////////////////////

Joystick* Joystick::create(const vector<string>& move,
                           const vector<string>& attack,
                           const vector<string>& skill)
{
    auto jo = new (std::nothrow) Joystick();
    if (jo && jo->init(move, attack, skill)) {
        jo->autorelease();
        return jo;
    }
    CC_SAFE_DELETE(jo);
    return nullptr;
}

bool Joystick::init(const vector<string>& move, const vector<string>& attack,
                    const vector<string>& skill)
{
    if (!Node::init()) return false;

    this->move = Joystick::_joystick_move::create(move);
    this->attack = Joystick::_joystick_attack::create(attack);
    this->skill = Joystick::_joystick_skill::create(skill);

    this->move->setAttack(this->attack);
    this->addChild(this->move, 0);
    this->addChild(this->attack, 1);
    this->addChild(this->skill, 2);

    return true;
}

Vec2 Joystick::getMoveAddPos() { return move->getAddPos(); }

Vec2 Joystick::getSkillAddPos() { return skill->getAddPos(); }

Vec2 Joystick::getAttackAddPos() { return attack->getAddPos(); }

void Joystick::setMoveOrgPos(const Vec2& pos) { move->setOrgPosition(pos); }

void Joystick::removeMoveTouchCatched() { move->removeTouchCatched(); }

void Joystick::removeSkillTouchCatched() { skill->removeTouchCatched(); }

void Joystick::removeAttackTouchCatched() { attack->removeTouchCatched(); }

void Joystick::setAttackPos(const Vec2& pos) { attack->setOrgPosition(pos); }

void Joystick::setSkillPos(const Vec2& pos) { this->skill->setPosition(pos); }

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

Joystick::_joystick_move* Joystick::_joystick_move::create(
    const vector<string>& move)
{
    auto jm = new (std::nothrow) _joystick_move();
    if (jm && jm->init(move)) {
        jm->autorelease();
        return jm;
    }
    CC_SAFE_DELETE(jm);
    return nullptr;
}

bool Joystick::_joystick_move::init(const vector<string>& move)
{
    this->button_bk = Sprite::createWithSpriteFrameName(move[0]);
    this->button = Sprite::createWithSpriteFrameName(move[1]);

    this->addChild(button_bk, 0);
    this->addChild(button, 1);

    this->schedule([&](float) { this->_update(); }, "_joystick_move_update");
    return true;
}

void Joystick::_joystick_move::_update()
{
    if (!switch_isCatched.isSet()) {
        auto touch =
            TouchesPool::getInstance()->getNearest_start(this->originalPos);
        if (touch && isInside(touch->getStartLocation())) {
            this->catched = touch;
            this->switch_isCatched.setSwitch(true);
            TouchesPool::getInstance()->setJoystickMoveCatched(touch);
        }
        button->setPosition(this->originalPos);
    } else {
        auto touchPos = catched->getLocation();
        [&]()  //防止为出现nan
        {
            if (MyMath::float_equal(touchPos.x, originalPos.x)) {
                touchPos.x += 1;
            }
        }();

        const float distance = MyMath::distance(originalPos, touchPos);
        if (distance <= this->button_bk->getContentSize().height / 2) {
            this->button->setPosition(touchPos);
        } else {
            Vec2 pos = MyMath::getPosOnLine(
                originalPos, touchPos,
                this->button_bk->getContentSize().height / 2);
            this->button->setPosition(pos);
        }

        auto gameManager = GameManager::getInstance();
        if (gameManager->isPause()) {
            //旋转英雄虚像
            auto hero = gameManager->getHeroPauseShow();
            if (this->button->getPosition() != this->originalPos) {
                const float rotation = MyMath::getRotation(
                    originalPos, this->button->getPosition());

                hero->setRotation(rotation);
            }
        } else {
            //旋转英雄
            auto hero = gameManager->getHero();
            if (this->button->getPosition() != this->originalPos) {
                const float rotation = MyMath::getRotation(
                    originalPos, this->button->getPosition());
                if (attack->isCannonCanRotate()) {
                    auto cannon = hero->getCannon();
                    cannon->setRotation(rotation);
                }
                hero->setRotation(rotation);
            }
        }
    }
}

void Joystick::_joystick_move::setOrgPosition(const Vec2& pos)
{
    this->originalPos = pos;
    this->button_bk->setPosition(pos);
}

Vec2 Joystick::_joystick_move::getAddPos()
{
    auto gameManager = GameManager::getInstance();
    if (gameManager->isHeroDead() || gameManager->isPause() ||
        !this->switch_isCatched.isSet())
        return Vec2::ZERO;

    auto buttonPos = this->button->getPosition();
    if (MyMath::float_equal(buttonPos.x, originalPos.x)) {
        buttonPos.x += 1;
    }

    Vec2 result;
    const float k = (this->originalPos.y - buttonPos.y) /
                    (this->originalPos.x - buttonPos.x);
    const float dis = this->button_bk->getContentSize().height / 2;

    float x = dis / sqrtf(POT(k) + 1);
    float y = 0.0f;
    if (buttonPos.x < originalPos.x) x = -x;

    y = k * x;

    result.x = x / dis;
    result.y = y / dis;

    return -result;
}

void Joystick::_joystick_move::setAttack(_joystick_attack* attack)
{
    this->attack = attack;
}

void Joystick::_joystick_move::removeTouchCatched()
{
    this->catched = nullptr;
    this->switch_isCatched.setSwitch(false);
}

bool Joystick::_joystick_move::isInside(const Vec2& pos)
{
    const float lenth = this->button_bk->getContentSize().height / 2;

    const float distance = MyMath::distance(pos, originalPos);

    return (distance <= lenth);
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

Joystick::_joystick_attack* Joystick::_joystick_attack::create(
    const vector<string>& attack)
{
    auto ja = new (std::nothrow) _joystick_attack();
    if (ja && ja->init(attack)) {
        ja->autorelease();
        return ja;
    }
    CC_SAFE_DELETE(ja);
    return nullptr;
}

bool Joystick::_joystick_attack::init(const vector<string>& attack)
{
    this->joystick_bk = Sprite::createWithSpriteFrameName(attack[0]);
    this->joystick = Sprite::createWithSpriteFrameName(attack[1]);
    this->addChild(joystick_bk, 0);
    this->addChild(joystick, 1);
    this->schedule([&](float) { this->_update(); },
                   "___joystick_attack_update");

    this->disable = Sprite::createWithSpriteFrameName(
        Imgs.gamePage0.p_joystick_attack_disable);
    this->addChild(disable, 1);

    this->switch_buttonDisbale.setSwitch(false);
    this->switch_canAttack.setSwitch(true);
    this->switch_cannonRotate.setSwitch(true);

    return true;
}

void Joystick::_joystick_attack::removeTouchCatched()
{
    this->catched = nullptr;
    this->switch_isCatched.setSwitch(false);
}

void Joystick::_joystick_attack::_update()
{
    auto gameManager = GameManager::getInstance();
    if (gameManager->isPause() || gameManager->isHeroDead()) return;

    if (!switch_isCatched.isSet()) {
        auto touch =
            TouchesPool::getInstance()->getNearest_start(this->originalPos);
        if (touch && isInside(touch->getStartLocation())) {
            this->catched = touch;
            this->switch_isCatched.setSwitch(true);
            TouchesPool::getInstance()->setJoystickAttackCatched(touch);
        }
        joystick->setPosition(this->originalPos);
        switch_buttonDisbale.setSwitch(false);
        disable->setVisible(false);
    } else {
        auto touchPos = catched->getLocation();

        if (MyMath::float_equal(touchPos.x, originalPos.x)) {
            touchPos.x += 1;
        }

        const float distance = MyMath::distance(originalPos, touchPos);
        if (distance <= this->joystick_bk->getContentSize().height / 2) {
            this->joystick->setPosition(touchPos);
        } else {
            Vec2 pos = MyMath::getPosOnLine(
                originalPos, touchPos,
                this->joystick_bk->getContentSize().height / 2);
            this->joystick->setPosition(pos);
        }

        auto hero = GameManager::getInstance()->getHero();
        // auto skill = hero->getSkill();

        ////使用技能时不攻击
        // if (skill->isRunSkill() && skill->getSkillType() ==
        // HeroSkillType::knife)
        //{
        //	this->switch_buttonDisbale.setSwitch(true);
        //	this->disable->setVisible(true);
        //}
        // else
        //{
        //	this->switch_buttonDisbale.setSwitch(false);
        //	this->disable->setVisible(false);
        //}

        if (switch_canAttack.isSet() && !switch_buttonDisbale.isSet()) {
            hero->shootBullet();
            switch_canAttack.setSwitch(false);
            switch_cannonRotate.setSwitch(false);

            this->unschedule("__attack_Delay");
            this->scheduleOnce(  //攻击冷却
                [&](float) { switch_canAttack.setSwitch(true); },
                hero->getAttackInterval(), "__attack_Delay");

            this->unschedule("__attack_canonRotate");
            this->scheduleOnce(  //跟随旋转冷却
                [&](float) { switch_cannonRotate.setSwitch(true); },
                hero->getAttackInterval() + 0.2f, "__attack_canonRotate");
        }
    }
}

bool Joystick::_joystick_attack::isInside(const Vec2& pos)
{
    const float lenth = this->joystick_bk->getContentSize().height / 2;

    const float distance = MyMath::distance(pos, joystick_bk->getPosition());

    return (distance <= lenth);
}

void Joystick::_joystick_attack::setOrgPosition(const Vec2& pos)
{
    this->originalPos = pos;
    this->joystick_bk->setPosition(pos);
    this->disable->setPosition(pos);
}

bool Joystick::_joystick_attack::isCannonCanRotate()
{
    return switch_cannonRotate.isSet();
}

Vec2 Joystick::_joystick_attack::getAddPos()
{
    if (GameManager::getInstance()->isHeroDead() ||
        GameManager::getInstance()->isPause() ||
        !this->switch_isCatched.isSet())
        return Vec2::ZERO;

    auto buttonPos = this->joystick->getPosition();

    if (MyMath::float_equal(buttonPos.x, originalPos.x)) {
        buttonPos.x += 1;
    }

    const float dis = this->joystick_bk->getContentSize().height / 2;

    Vec2 result;
    result.x = (buttonPos.x - originalPos.x) / dis;
    result.y = (buttonPos.y - originalPos.y) / dis;

    return result;
}

////////////////////////////////////////////////////////////////////

Joystick::_joystick_skill* Joystick::_joystick_skill::create(
    const vector<string>& skill)
{
    auto sk = new (std::nothrow) Joystick::_joystick_skill();
    if (sk && sk->init(skill)) {
        sk->autorelease();
        return sk;
    }
    CC_SAFE_DELETE(sk);
    return nullptr;
}

bool Joystick::_joystick_skill::init(const vector<string>& skill)
{
    this->menu = Menu::create();
    this->menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    auto sp0 = Sprite::createWithSpriteFrameName(skill[2]);
    auto sp1 = Sprite::createWithSpriteFrameName(skill[3]);
    auto sp2 = Sprite::createWithSpriteFrameName(skill[4]);

    this->button = MenuItemSprite::create(sp0, sp1, sp2);
    menu->addChild(button, 0);

    this->button->setCallback([&](Ref*) {
        auto hero = GameManager::getInstance()->getHero();
        auto skill = hero->getSkill();
        if (skill->isFinishCountDown()) {
            skill->runSkill();

            auto call = [&]() {
                rand_int r0(0, Imgs.onGame.skillShield.vp_effect.size() -
                                   1);  //范围可能改变,不能设置成 static
                auto particle = ParticleOnExplode::create(
                    Imgs.onGame.skillShield.vp_effect[r0()],
                    MyDefault.skillShield.particleLiveTime_onStart);
                if (particle) {
                    auto pos = button->getPosition();
                    particle->setPosition(pos);
                    GameManager::getInstance()->addParticleChild(particle);
                }
            };

            if (MyDefault.option.particleQuality != 0) {
                auto v = static_cast<unsigned int>(MyMath::floatToInt(
                    6 * static_cast<float>(MyDefault.option.particleQuality /
                                           2.0f)));
                for (size_t x = 0; x < v; ++x) {
                    call();
                }
            }
        }
    });

    this->schedule([&](float) { _update(); }, "_uodate");

    this->timeShow =
        Label::createWithTTF("", MyDefault.font.medium, 48, Size(100, 0),
                             TextHAlignment::CENTER, TextVAlignment::CENTER);
    timeShow->setTextColor(Color4B(20, 20, 20, 255));

    this->addChild(timeShow, 1);

    this->joystick_button = Sprite::createWithSpriteFrameName(skill[0]);
    joystick_button->setVisible(false);
    this->joystick_bk = Sprite::createWithSpriteFrameName(skill[1]);
    joystick_bk->setVisible(false);

    /*this->joystick_arrow =
    Sprite::createWithSpriteFrameName(Imgs.gamePage0.p_joystick_arrow);
    joystick_arrow->setVisible(false);
    joystick_arrow->setTag(tag_heroSkillJoystickArrow);
    GameManager::getInstance()->addOthersChild(joystick_arrow);*/

    this->addChild(joystick_button, 3);
    this->addChild(joystick_bk, 2);

    return true;
}

void Joystick::_joystick_skill::setPosition(const Vec2& pos)
{
    this->button->setPosition(pos);
    this->timeShow->setPosition(pos);
    this->joystick_bk->setPosition(pos);
    this->joystick_button->setPosition(pos);

    // joystick_arrow->setAnchorPoint(Vec2(0, 0.5));
    // joystick_arrow->setPosition(GameManager::getInstance()->getHero()->getPosition());

    this->originalPos = pos;
}

Vec2 Joystick::_joystick_skill::getAddPos()
{
    if (GameManager::getInstance()->isHeroDead() ||
        GameManager::getInstance()->isPause() ||
        !this->switch_isCatched.isSet())
        return Vec2::ZERO;

    auto buttonPos = this->joystick_button->getPosition();

    if (MyMath::float_equal(this->originalPos.x, buttonPos.x))
        return Vec2::ZERO;

    const float dis = this->joystick_bk->getContentSize().height / 2;

    Vec2 result;
    result.x = (buttonPos.x - originalPos.x) / dis;
    result.y = (buttonPos.y - originalPos.y) / dis;

    return result;
}

void Joystick::_joystick_skill::removeTouchCatched()
{
    this->catched = nullptr;
    this->switch_isCatched.setSwitch(false);
}

bool Joystick::_joystick_skill::isInside(const Vec2& pos)
{
    const float lenth = this->joystick_bk->getContentSize().height / 2;

    const float distance = MyMath::distance(pos, button->getPosition());

    return (distance <= lenth);
}

void Joystick::_joystick_skill::updateJoystick()
{
    if (!switch_isCatched.isSet()) {
        auto touch =
            TouchesPool::getInstance()->getNearest_start(this->originalPos);
        if (touch && isInside(touch->getStartLocation())) {
            this->catched = touch;
            this->switch_isCatched.setSwitch(true);
            TouchesPool::getInstance()->setJoystickSkillCatched(touch);
        }
        joystick_button->setPosition(this->originalPos);
    } else {
        auto touchPos = catched->getLocation();
        //防止为直线从而出现nan
        if (MyMath::float_equal(touchPos.x, originalPos.x)) {
            touchPos.x += 2;
        }

        const float distance = MyMath::distance(originalPos, touchPos);
        if (distance <= this->joystick_bk->getContentSize().height / 2) {
            this->joystick_button->setPosition(touchPos);
        } else {
            Vec2 pos = MyMath::getPosOnLine(
                originalPos, touchPos,
                this->joystick_bk->getContentSize().height / 2);
            this->joystick_button->setPosition(pos);
        }
    }
}

void Joystick::_joystick_skill::_update()
{
    auto hero = GameManager::getInstance()->getHero();
    auto skill = hero->getSkill();

    if (skill->isRunSkill() || !skill->isFinishCountDown()) {
        this->button->setEnabled(false);
    } else {
        this->joystick_button->setVisible(true);
        this->button->setEnabled(true);
    }

    if (!skill->isRunSkill() && !skill->isFinishCountDown()) {
        timeShow->setVisible(true);
        char str[8] = "";
        if (skill->getCountDown() < 1) {
            std::snprintf(str, sizeof(str), "%.1f", skill->getCountDown());
        } else {
            std::snprintf(str, sizeof(str), "%d",
                          static_cast<int>(skill->getCountDown()));
        }

        this->timeShow->setString(str);
    } else {
        timeShow->setVisible(false);
    }

    if (skill->isRunSkill()) {
        joystick_button->setVisible(true);
        joystick_bk->setVisible(true);
        // joystick_arrow->setVisible(false);
        /*if (skill->getSkillType() == HeroSkillType::knife)
        {
                auto addPos = this->getAddPos();
                if (addPos != Vec2::ZERO)
                {
                        joystick_arrow->setVisible(true);
                        auto r = MyMath::getRotation(Vec2(0, 0), addPos);
                        joystick_arrow->setRotation(r);
                }
                else
                {
                        joystick_arrow->setVisible(false);
                }
        }*/
    } else {
        joystick_button->setVisible(false);
        joystick_bk->setVisible(false);
        // joystick_arrow->setVisible(false);
    }

    this->updateJoystick();

    if (GameManager::getInstance()->isPause() ||
        GameManager::getInstance()->isHeroDead()) {
        this->button->setEnabled(false);
    }
}
