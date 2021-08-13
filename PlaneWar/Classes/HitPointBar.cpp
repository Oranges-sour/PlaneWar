#include "header/HitPointBar.h"

#include <string>
using namespace std;

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"

HitPointBar_forEnemy* HitPointBar_forEnemy::create(
    base_attackEnemy* enemy, const string& hitPointBar_Plist,
    const string& hitPointBar_background_Plist)
{
    auto _bar = new (std::nothrow) HitPointBar_forEnemy();
    if (_bar && enemy) {
        //创建Bar
        _bar->hitPointBar = LoadingBar::create(
            hitPointBar_Plist, Widget::TextureResType::PLIST, 100.0f);
        _bar->hitPointBar_background =
            Sprite::createWithSpriteFrameName(hitPointBar_background_Plist);
        _bar->hitPointBar->setOpacity(0);
        _bar->hitPointBar_background->setOpacity(0);

        _bar->setTag(tag_enemyHitPointBar);
        _bar->addChild(_bar->hitPointBar_background, 0);
        _bar->addChild(_bar->hitPointBar, 1);
        _bar->init(enemy);

        _bar->autorelease();
        return _bar;
    }
    CC_SAFE_DELETE(_bar);
    return nullptr;
}

void HitPointBar_forEnemy::setPosition(const Vec2& position)
{
    this->hitPointBar->setPosition(position);
    this->hitPointBar_background->setPosition(position);
}

void HitPointBar_forEnemy::_update()
{
    float fullHitPoint = enemy->getFullHitPoint();
    float nowHitPoint = enemy->getHitPoint();

    float percent = nowHitPoint / fullHitPoint * 100;
    this->hitPointBar->setPercent(percent);
}

bool HitPointBar_forEnemy::init(base_attackEnemy* enemy)
{
    if (!Node::init()) {
        return false;
    }

    this->enemy = enemy;

    this->setPosition(
        Vec2(enemy->getPosition().x,
             enemy->getPosition().y - enemy->getContentSize().height / 2 - 15));

    this->schedule([&](float) { this->_update(); }, "_update");
    return true;
}

void HitPointBar_forEnemy::fadeInBar()
{
    auto fadeIn = FadeIn::create(0.2f);
    this->hitPointBar->stopAllActions();
    this->hitPointBar_background->stopAllActions();
    this->hitPointBar->runAction(fadeIn->clone());
    this->hitPointBar_background->runAction(fadeIn->clone());
}

void HitPointBar_forEnemy::fadeOutBar()
{
    auto fadeOut = FadeOut::create(0.3f);
    this->hitPointBar->stopAllActions();
    this->hitPointBar_background->stopAllActions();
    this->hitPointBar->runAction(fadeOut->clone());
    this->hitPointBar_background->runAction(fadeOut->clone());
}

//////////////////////////////////////////////////////////////////////////

/*
HitPointBar_Hero* HitPointBar_Hero::create(const string& bar_front, const
string& energy, const string& hitpoint, const string& bar_bk)
{
        auto _bar = new ( std::nothrow)HitPointBar_Hero();
        if (_bar && _bar->init())
        {
                _bar->bar_hp = LoadingBar::create(hitpoint,
Widget::TextureResType::PLIST, 100.0f); _bar->bar_ep =
LoadingBar::create(energy, Widget::TextureResType::PLIST, 100.0f);
                _bar->bar_front = Sprite::createWithSpriteFrameName(bar_front);
                _bar->bar_bk= Sprite::createWithSpriteFrameName(bar_bk);
                /////////////////////////////////////////////////////////////////////
                _bar->addChild(_bar->bar_bk, 0);
                _bar->addChild(_bar->bar_hp, 1);
                _bar->addChild(_bar->bar_ep, 1);
                _bar->addChild(_bar->bar_front, 2);

                _bar->autorelease();
                return _bar;
        }
        CC_SAFE_DELETE(_bar);
        return nullptr;
}

void HitPointBar_Hero::setHero(Hero* hero)
{
        if (hero)
        {
                this->hero = hero;

                //设置坐标
                Vec2 pos_front_bk = Vec2(hero->getPosition().x,
hero->getPosition().y + hero->getContentSize().width);
                this->bar_front->setPosition(pos_front_bk);
                this->bar_bk->setPosition(pos_front_bk);

                this->bar_hp->setPosition(Vec2(pos_front_bk.x, pos_front_bk.y +
7)); this->bar_ep->setPosition(Vec2(pos_front_bk.x, pos_front_bk.y - 12));
        }

}

void HitPointBar_Hero::setFullPoint(int hitPoint, int energyPoint)
{
        hp_now = hp_should = fullHP = hitPoint;
        ep_now = ep_should = fullEP = energyPoint;
}

void HitPointBar_Hero::setHitPoint(float hp)
{
        if (hp < 0)
                hp_should = 0;
        else if (hp > fullHP)
                hp_should = fullHP;
        else
                hp_should = hp;
}

void HitPointBar_Hero::setEnergyPoint(float ep)
{
        if (ep < 0)
                ep_should = 0;
        else if (ep > fullEP)
                ep_should = fullEP;
        else
                ep_should = ep;
}

bool HitPointBar_Hero::isAlive()
{
        if (hp_now > 0)
                return true;
        else
                return false;
}

int HitPointBar_Hero::getHitPoint()
{
        return hp_now;
}

int HitPointBar_Hero::getEnergyPoint()
{
        return ep_now;
}

void HitPointBar_Hero::updateBar(float)
{
        //更新血条
        float hp_percent = 0;
        hp_percent = (hp_now / fullHP) * 100;
        if (hp_percent < 0)
                hp_percent = 0;

        this->bar_hp->setPercent(hp_percent);

        if (hp_now > hp_should)
        {
                if (hp_now - hp_should > 2)
                        hp_now -= 2;
                else
                        hp_now -= 1;
        }
        if (hp_now < hp_should)
        {
                if (hp_should - hp_now > 2)
                        hp_now += 2;
                else
                        hp_now += 1;
        }

        //更新蓝条
        float ep_percent = 0;
        ep_percent = (ep_now / fullEP) * 100;

        if (ep_percent < 0)
                ep_percent = 0;
        this->bar_ep->setPercent(ep_percent);

        if (ep_now > ep_should)
        {
                if (ep_now - ep_should > 2)
                        ep_now -= 2;
                else
                        ep_now -= 1;
        }
        if (ep_now < ep_should)
        {
                if (ep_should - ep_now > 2)
                        ep_now += 2;
                else
                        ep_now += 1;
        }
}

void HitPointBar_Hero::removeFromParent()
{
        bar_hp->removeFromParent();
        bar_ep->removeFromParent();
        bar_front->removeFromParent();
        bar_bk->removeFromParent();

        Node::removeFromParent();
}

bool HitPointBar_Hero::init()
{
        this->schedule(schedule_selector(HitPointBar_Hero::updateBar));
        return true;
}

*/
