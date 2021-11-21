#include "header/HeroSkill.h"

#include "header/Bullet.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/Joystick.h"
#include "header/MyMath.h"
#include "header/Particle.h"
#include "header/Random.h"

HeroSkill_shield* HeroSkill_shield::create()
{
    auto s = new (std::nothrow) HeroSkill_shield();
    if (s && s->init()) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

void HeroSkill_shield::getAttack(float hitPoint)
{
    if (this->hitPoint - hitPoint < 0) {
        this->hitPoint = 0;
    } else {
        this->hitPoint -= hitPoint;
    }

    if (MyDefault.option.particleQuality != 0) {
        auto p = GLProgramCache::getInstance()->getGLProgram(
            MyDefault.myShader.light);
        auto ps = GLProgramState::create(p);
        ps->setUniformFloat("light", 1.4f);
        shield->setGLProgram(p);
        shield->setGLProgramState(ps);

        this->unschedule("_glProgramBack");
        this->scheduleOnce(
            [&](float) {
                auto p = GLProgramCache::getInstance()->getGLProgram(
                    MyDefault.myShader.defaultShader);
                shield->setGLProgram(p);
            },
            0.02f, "_glProgramBack");
    }
}

void HeroSkill_shield::setMaxHitPoint(float hitPoint)
{
    this->maxHitPoint = hitPoint;
}

float HeroSkill_shield::getMaxHitPoint() { return maxHitPoint; }

void HeroSkill_shield::stopSkill()
{
    HeroSkill_base::stopSkill();

    this->unschedule("subHitPoint");
    this->unschedule("_update");
    this->unschedule("_showParticleOnMove");
    this->unschedule("_glProgramBack");

    //设置技能冷却
    this->setIsFinishCountDown(false);
    this->setCountDown(this->getFullCountDown());
    this->schedule(
        [&](float) {
            if (GameManager::getInstance()->isPause() ||
                GameManager::getInstance()->isHeroDead())
                return;
            auto frameRate = Director::getInstance()->getFrameRate();

            float nowCountDown = getCountDown();
            if (nowCountDown > 0) {
                float sub = 1.0f / frameRate;

                if (nowCountDown - sub < 0) {
                    setCountDown(0);
                } else {
                    setCountDown(nowCountDown - sub);
                }
            } else {
                this->unschedule("_runCountDown");
                this->setIsFinishCountDown(true);
            }
        },
        "_runCountDown");
    ///////////////////////////////////

    auto act = FadeOut::create(0.25f);
    auto func = CallFuncN::create([&](Node* n) { n->removeFromParent(); });
    auto run = Sequence::create(act, func, nullptr);

    this->skillBK->runAction(run->clone());
    this->shield->getPhysicsBody()->setEnabled(false);
    this->shield->runAction(run->clone());
    this->bar->runAction(run->clone());
    this->bar_bk->runAction(run->clone());

    for (int x = 0; x < 2; ++x) {
        auto shake = Sprite::create(Imgs.onGame.skillShield.t_shake);
        shake->setScale((shield->getContentSize().width / 2) /
                        (shake->getContentSize().width / 2));
        shake->setPosition(shield->getPosition());
        GameManager::getInstance()->addSkillEffectChild(shake);

        const float time = 1.0f - x * 0.3f;
        auto shakeRun = ScaleTo::create(time, 1.0f);
        auto shakeFade = FadeOut::create(time);
        auto r0 = EaseOut::create(shakeRun, 2.0f);
        auto r1 = EaseOut::create(shakeFade, 2.0f);

        auto act0 = Spawn::create(r0, r1, nullptr);
        auto func = CallFuncN::create([&](Node* n) { n->removeFromParent(); });
        auto run = Sequence::create(act0, func, nullptr);
        shake->runAction(run);
    }

    skillBK = nullptr;
    shield = nullptr;
    bar = nullptr;
    bar_bk = nullptr;

    const float serchRange = 650;

    auto heroPos = GameManager::getInstance()->getHero()->getPosition();
    auto enemies =
        GameManager::getInstance()->getEnemyInRange(heroPos, serchRange);

    auto bullets =
        GameManager::getInstance()->getEnemyBulletInRange(heroPos, serchRange);
    for (auto& it : bullets) {
        it->kill();
    }

    const float damage = 80.0f;
    const float pushDistance = 300.0f;
    for (auto& it : enemies) {
        const float dis = MyMath::distance(heroPos, it->getPosition()) - 5;
        float damageRate = 0;
        float pushRate = 0;

        float temp0 = serchRange * 0.5;
        if (dis < temp0) {
            damageRate = 1;
        } else {
            damageRate = 1 - (dis - temp0) / (serchRange - temp0);
        }

        float temp1 = serchRange * 0.3;
        if (dis < temp1) {
            pushRate = 1;
        } else {
            pushRate = 1 - (dis - temp1) / (serchRange - temp1);
        }

        it->getAttack(damage * damageRate);

        if (it->getEnemyType() == EnemyType::attack) {
            auto enemy = dynamic_cast<base_attackEnemy*>(it);
            if (enemy->isOnAttack()) {
                enemy->shake(0.5f);

                auto enemyPos = enemy->getPosition();
                if (enemyPos.x == heroPos.x) {
                    enemyPos.x -= 2;
                }
                float k = (enemyPos.y - heroPos.y) / (enemyPos.x - heroPos.x);
                Vec2 p0;
                p0.x = (pushDistance * pushRate) / sqrtf(POT(k) + 1);
                if (enemyPos.x < heroPos.x) {
                    p0.x = -p0.x;
                }

                p0.y = k * p0.x;
                auto act = MoveBy::create(0.2f, p0);
                auto r0 = EaseOut::create(act, 2.0f);
                enemy->runAction(r0);
            }
        }
    }

    //创建粒子
    auto call = [&]() {
        rand_int r0(0, Imgs.onGame.skillShield.vp_effect.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            Imgs.onGame.skillShield.vp_effect[r0()],
            MyDefault.skillShield.particleLiveTime_onStart, 2);
        if (particle) {
            auto heroPos = GameManager::getInstance()->getHero()->getPosition();
            particle->setPosition(heroPos);
            particle->setTag(tag_skillShieldEffect);
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        int v =
            MyMath::floatToInt(MyDefault.skillShield.particleQuantity_onStart *
                               MyDefault.option.particleQuality / 2.0f);
        for (int x = 0; x < v; ++x) {
            call();
        }
    }
}

void HeroSkill_shield::showParticleOnStart()
{
    auto call = [&]() {
        rand_int r0(0, Imgs.onGame.skillShield.vp_effect.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            Imgs.onGame.skillShield.vp_effect[r0()],
            MyDefault.skillShield.particleLiveTime_onStart, 2.0f);
        if (particle) {
            auto heroPos = GameManager::getInstance()->getHero()->getPosition();
            particle->setPosition(heroPos);
            particle->setTag(tag_skillShieldEffect);
            particle->setOpacity(0.8 * 255);
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        int v =
            MyMath::floatToInt(MyDefault.skillShield.particleQuantity_onStart *
                               MyDefault.option.particleQuality / 2.0f);
        for (int x = 0; x < v; ++x) {
            call();
        }
    }
}

void HeroSkill_shield::runSkill()
{
    auto hero = GameManager::getInstance()->getHero();
    if (hero->getEnergyPoint() - 30 < 0) {  //耗蓝
        return;
    }
    if (this->isRunSkill()) {
        this->stopSkill();
    }
    hero->setEnergyPoint(hero->getEnergyPoint() - 25);

    auto heroPos = hero->getPosition();
    HeroSkill_base::runSkill();

    this->schedule(
        [&](float) {
            if (GameManager::getInstance()->isPause() ||
                GameManager::getInstance()->isHeroDead())
                return;

            auto frameRate = Director::getInstance()->getFrameRate();
            this->hitPoint -= 80 / frameRate;
        },
        "subHitPoint");

    this->schedule([&](float) { this->_update(); }, "_update");

    this->schedule([&](float) { this->showParticleOnMove(); }, 0.2f,
                   "_showParticleOnMove");

    this->showParticleOnStart();

    this->hitPoint = maxHitPoint;

    //////////////////////////////////////////////////////////////////////////
    const auto winSize = Director::getInstance()->getVisibleSize();
    this->skillBK = Sprite::create(Imgs.onGame.skillShield.t_background);
    skillBK->setPosition(winSize / 2);
    skillBK->setOpacity(0);
    //////////////////////////////////////////////////////////////////////////
    this->shield = Shield::create(Imgs.onGame.skillShield.p_shield);

    shield->setSkill(this);

    shield->setPosition(heroPos);
    shield->setOpacity(0);

    auto shieldAct = ScaleTo::create(0.1f, 1.0f);
    shield->setScale(0);
    shield->runAction(shieldAct);

    //////////////////////////////////////////////////////////////////////////
    this->bar = LoadingBar::create(Imgs.onGame.skillShield.p_bar,
                                   Widget::TextureResType::PLIST, 100);
    bar->setPosition(Vec2(heroPos.x, heroPos.y + 150));
    bar->setOpacity(0);

    //////////////////////////////////////////////////////////////////////////
    this->bar_bk =
        Sprite::createWithSpriteFrameName(Imgs.onGame.skillShield.p_bar_bk);
    bar_bk->setPosition(Vec2(heroPos.x, heroPos.y + 150));
    bar_bk->setOpacity(0);

    //////////////////////////////////////////////////////////////////////////
    GameManager::getInstance()->addSkillEffectChild(skillBK);
    GameManager::getInstance()->addSkillEffectChild(shield);
    GameManager::getInstance()->addSkillEffectChild(bar_bk);
    GameManager::getInstance()->addSkillEffectChild(bar);

    auto run = FadeIn::create(0.15f);

    this->skillBK->runAction(run->clone());
    this->shield->runAction(run->clone());
    this->bar->runAction(run->clone());
    this->bar_bk->runAction(run->clone());
}

void HeroSkill_shield::_update()
{
    float percent = hitPoint / maxHitPoint;
    bar->setPercent(percent * 100);

    if (hitPoint <= 0) {
        this->stopSkill();
    }
}

void HeroSkill_shield::showParticleOnMove()
{
    if (GameManager::getInstance()->isPause()) return;

    auto call = [&]() {
        rand_int r0(0, Imgs.onGame.skillShield.vp_effect.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            Imgs.onGame.skillShield.vp_effect[r0()],
            MyDefault.skillShield.particleLiveTime_onMove);
        if (particle) {
            auto heroPos = GameManager::getInstance()->getHero()->getPosition();
            particle->setPosition(heroPos);
            particle->setTag(tag_skillShieldEffect);
            particle->setOpacity(0.8 * 255);
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        call();
    }
}

HeroSkill_shield::Shield* HeroSkill_shield::Shield::create(const string& frame)
{
    auto s = new (std::nothrow) HeroSkill_shield::Shield();
    if (s && s->initWithSpriteFrameName(frame)) {
        s->setTag(tag_skillShield);
        PhysicsShapeCache::getInstance()->setBodyOnSprite(
            MyDefault.skillShield.physicsInfo, s);
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

void HeroSkill_shield::Shield::setSkill(HeroSkill_shield* skill)
{
    this->skill = skill;
}

void HeroSkill_shield::Shield::getAttack(float damage)
{
    this->skill->getAttack(damage);
}

void HeroSkill_shield::Shield::_update() {}

//////////////////////////////////////////////////////////////////////////

#if 1

//废弃的

#else

HeroSkill_knife* HeroSkill_knife::create()
{
    auto s = new (std::nothrow) HeroSkill_knife();
    if (s && s->init()) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

void HeroSkill_knife::runSkill()
{
    auto hero = GameManager::getInstance()->getHero();
    if (hero->getEnergyPoint() - 30 < 0) {  //耗蓝
        return;
    }
    if (this->isRunSkill()) {
        this->stopSkill();
    }
    hero->setEnergyPoint(hero->getEnergyPoint() - 30);

    auto heroPos = hero->getPosition();
    HeroSkill_base::runSkill();

    this->nowValue = maxValue;

    //进度条减少
    this->schedule(
        [&](float) {
            if (GameManager::getInstance()->isPause() ||
                GameManager::getInstance()->isHeroDead())
                return;

            auto frameRate = Director::getInstance()->getFrameRate();
            this->nowValue -= 20 / frameRate;
        },
        "subValue");

    for (int x = 0; x < 5; ++x) {
        this->weapon->attack(Vec2::ZERO);
    }

    this->schedule([&](float) { this->_update(); }, "_update");

    //////////////////////////////////////////////////////////////////////////
    auto winSize = Director::getInstance()->getVisibleSize();
    this->skillBK = Sprite::create(Imgs.onGame.skillShield.t_background);
    skillBK->setPosition(winSize / 2);
    skillBK->setOpacity(0);

    //////////////////////////////////////////////////////////////////////////
    this->bar = LoadingBar::create(Imgs.onGame.skillShield.p_bar,
                                   Widget::TextureResType::PLIST, 100);
    bar->setPosition(Vec2(heroPos.x, heroPos.y + 150));
    bar->setOpacity(0);

    //////////////////////////////////////////////////////////////////////////
    this->bar_bk =
        Sprite::createWithSpriteFrameName(Imgs.onGame.skillShield.p_bar_bk);
    bar_bk->setPosition(Vec2(heroPos.x, heroPos.y + 150));
    bar_bk->setOpacity(0);

    //////////////////////////////////////////////////////////////////////////
    GameManager::getInstance()->addSkillEffectChild(skillBK);
    GameManager::getInstance()->addSkillEffectChild(bar_bk);
    GameManager::getInstance()->addSkillEffectChild(bar);

    auto run = FadeIn::create(0.15f);

    this->bar->runAction(run->clone());
    this->bar_bk->runAction(run->clone());
    this->skillBK->runAction(run->clone());
}

void HeroSkill_knife::stopSkill()
{
    HeroSkill_base::stopSkill();
    // this->unschedule("attack");
    this->unschedule("_update");
    this->unschedule("subValue");

    //设置技能冷却
    this->setIsFinishCountDown(false);
    this->setCountDown(this->getFullCountDown());
    this->schedule(
        [&](float) {
            if (GameManager::getInstance()->isPause()) return;
            auto frameRate = Director::getInstance()->getFrameRate();

            if (getCountDown() >= 0) {
                setCountDown(getCountDown() - 1.0f / frameRate);
            } else {
                setCountDown(0.0f);
                this->unschedule("_runCountDown");
                this->setIsFinishCountDown(true);
            }
        },
        "_runCountDown");
    ///////////////////////////////////

    auto act = FadeOut::create(0.25f);
    auto func = CallFuncN::create([&](Node* n) { n->removeFromParent(); });
    auto run = Sequence::create(act, func, nullptr);

    this->skillBK->runAction(run->clone());
    this->bar->runAction(run->clone());
    this->bar_bk->runAction(run->clone());

    skillBK = nullptr;
    bar = nullptr;
    bar_bk = nullptr;

    //创建粒子
    auto call = [&]() {
        auto& e = Random::getEngine();
        rand_int r0(0, Imgs.onGame.skillShield.vp_effect.size() -
                           1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            Imgs.onGame.skillShield.vp_effect[r0(e)],
            MyDefault.skillShield.particleLiveTime_onStart, 2.0f);
        if (particle) {
            auto heroPos = GameManager::getInstance()->getHero()->getPosition();
            particle->setPosition(heroPos);
            particle->setTag(tag_skillShieldEffect);
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    if (MyDefault.option.particleQuality != 0) {
        auto v = static_cast<unsigned int>(MyMath::floatToInt(
            MyDefault.skillShield.particleQuantity_onStart *
            static_cast<float>(MyDefault.option.particleQuality / 2.0f)));
        for (size_t x = 0; x < v; ++x) {
            call();
        }
    }
}

void HeroSkill_knife::setWeapon(Weapon_HeroSkill* weapon)
{
    if (weapon) {
        this->addChild(weapon);
        this->weapon = weapon;
    }
}

void HeroSkill_knife::_update()
{
    float percent = nowValue / maxValue;
    bar->setPercent(percent * 100);

    if (nowValue < 0) {
        this->stopSkill();
    }
}

#endif
