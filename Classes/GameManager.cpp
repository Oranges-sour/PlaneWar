#include "header/GameManager.h"

#include <cstdio>
#include <future>
#include <limits>  //一些数据的极值
#include <map>
#include <thread>
#include <vector>
using namespace std;

#include "header/Bullet.h"
#include "header/Enemy.h"
#include "header/EnemyGroup.h"
#include "header/Hero.h"
#include "header/Joystick.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/MyThreads.h"
#include "header/ObjectPool.h"
#include "header/Particle.h"
#include "header/PhysicsShapeCache.h"
#include "header/Random.h"
#include "header/XMLManager.h"

GameManager* GameManager::instance = nullptr;

enum _GameManager_zOrder {
    z_bkMap,
    z_gameMap,
    z_others,
    z_enemy,
    z_enemyGroup,
    z_hero,
    z_heroPauseShow,
    z_heroCannon,
    z_enemyPointer,
    z_particle,
    z_bullet,
    z_redBK,
    z_skillEffect,
    z_barBK,
    z_barUI,
    z_barLabel,
    z_barFront,
    z_joystick
};

GameManager::GameManager() {}

GameManager::~GameManager() {}

bool GameManager::createGameMap(const string& map_texture)
{
    gameMap = Sprite::createWithTexture(
        Director::getInstance()->getTextureCache()->getTextureForKey(
            map_texture));
    if (gameMap) {
        gameMap->setPosition(Director::getInstance()->getVisibleSize() / 2);
        // gameMap->setVisible(false);
        this->addChild(gameMap, _GameManager_zOrder::z_gameMap);

        if (randMap0) {
            delete randMap0;
            randMap0 = nullptr;
        }

        if (randMap1) {
            delete randMap1;
            randMap1 = nullptr;
        }

        randMap0 = new rand_int(-gameMap->getContentSize().width / 2,
                                gameMap->getContentSize().width / 2);
        randMap1 = new rand_int(-gameMap->getContentSize().height / 2,
                                gameMap->getContentSize().height / 2);
        return true;
    }
    return false;
}

bool GameManager::createGameBkground(const string& bk_texture)
{
    this->bkMap = Sprite::createWithTexture(
        Director::getInstance()->getTextureCache()->getTextureForKey(
            bk_texture));
    if (bkMap) {
        bkMap->setPosition(Director::getInstance()->getVisibleSize() / 2);
        // bkMap->setVisible(false);
        this->addChild(bkMap, _GameManager_zOrder::z_bkMap);
        return true;
    }
    return false;
}

bool GameManager::createJoystick(const vector<string>& move,
                                 const vector<string>& attack,
                                 const vector<string>& skill)
{
    this->joystick = Joystick::create(move, attack, skill);
    if (joystick) {
        this->addChild(joystick, _GameManager_zOrder::z_joystick);
        return true;
    }
    return false;
}

bool GameManager::createHero(const string& hero_plist,
                             const string& cannon_plist,
                             const vector<string>& hero_explode_plist,
                             const string& physicsInfo)
{
    this->hero = Hero::create(hero_plist, cannon_plist);
    this->heroExplode = hero_explode_plist;
    if (hero) {
        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, hero);
        hero->setTag(tag_hero);
        hero->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(hero->getCannon(), _GameManager_zOrder::z_heroCannon);
        this->addChild(hero, _GameManager_zOrder::z_hero);
    }

    this->heroPauseShow = Sprite::createWithSpriteFrameName(hero_plist);
    heroPauseShow->setOpacity(0.8f * 255);
    heroPauseShow->setVisible(false);
    heroPauseShow->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(heroPauseShow, _GameManager_zOrder::z_heroPauseShow);

    return false;
}

bool GameManager::createEnemyPointer(const string& pointer)
{
    this->enemyPointer = Sprite::createWithSpriteFrameName(pointer);
    if (enemyPointer) {
        this->addChild(enemyPointer, _GameManager_zOrder::z_enemyPointer);

        constexpr float time = 0.5f;
        auto act1 = FadeIn::create(time - 0.15f);
        auto act2 = FadeOut::create(time);
        auto delay = DelayTime::create(0.3f);
        auto s = Sequence::create(act1, delay, act2, nullptr);
        enemyPointer->runAction(RepeatForever::create(s));

        return true;
    }
    return false;
}

bool GameManager::createRedBK(const string& bk_texture)
{
    this->redBk = Sprite::createWithTexture(
        Director::getInstance()->getTextureCache()->getTextureForKey(
            bk_texture));
    if (redBk) {
        redBk->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(redBk, _GameManager_zOrder::z_redBK);
        return true;
    }
    return false;
}

bool GameManager::createBar(const string& bar_bk, const string& bar_front,
                            const string& hp, const string& ep,
                            const string& progress, const string& font)
{
    this->bar_bk = Sprite::createWithSpriteFrameName(bar_bk);

    for (auto& it : this->bar_front) {
        it = Sprite::createWithSpriteFrameName(bar_front);
    }
    for (auto& it : this->bar_label) {
        it = Label::createWithTTF("", font, 35, Size(250, 0),
                                  TextHAlignment::CENTER,
                                  TextVAlignment::CENTER);
        it->setTextColor(Color4B(200, 200, 200, 255));
    }

    bar_ui[0] = LoadingBar::create(hp, Widget::TextureResType::PLIST, 100);
    bar_ui[1] = LoadingBar::create(ep, Widget::TextureResType::PLIST, 100);
    bar_ui[2] =
        LoadingBar::create(progress, Widget::TextureResType::PLIST, 100);

    this->addChild(this->bar_bk, _GameManager_zOrder::z_barBK);

    for (auto& it : this->bar_front) {
        this->addChild(it, _GameManager_zOrder::z_barFront);
    }
    for (auto& it : this->bar_label) {
        this->addChild(it, _GameManager_zOrder::z_barLabel);
    }
    for (auto& it : this->bar_ui) {
        this->addChild(it, _GameManager_zOrder::z_barUI);
    }
    return true;
}

bool GameManager::isHeroDead() { return switch_heroIsDeid.isSet(); }

bool GameManager::isPause() { return switch_isPause.isSet(); }

void GameManager::setIsPause(bool v)
{
    switch_isPause.setSwitch(v);

    if (this->isPause()) {
        for (auto& it : this->particle_parent->getChildren()) {
            it->pause();
        }
        for (auto& it : this->bullet_parent->getChildren()) {
            it->pause();
        }
        for (auto& it : this->enemy_parent->getChildren()) {
            it->pause();
        }
        for (auto& it : this->others_parent->getChildren()) {
            it->pause();
        }
        for (auto& it : this->skillEffect_parent->getChildren()) {
            it->pause();
        }

        enemyPointer->pause();

        this->heroPauseShow->setVisible(true);
        this->heroPauseShow->setRotation(hero->getRotation());
    } else {
        for (auto& it : this->particle_parent->getChildren()) {
            it->resume();
        }
        for (auto& it : this->bullet_parent->getChildren()) {
            it->resume();
        }
        for (auto& it : this->enemy_parent->getChildren()) {
            it->resume();
        }
        for (auto& it : this->others_parent->getChildren()) {
            it->resume();
        }
        for (auto& it : this->skillEffect_parent->getChildren()) {
            it->resume();
        }

        enemyPointer->resume();
        this->heroPauseShow->setVisible(false);
        hero->setRotation(heroPauseShow->getRotation());
    }
}

void GameManager::_update()
{
    if (this->isPause()) {
        return;
    }

    this->uptateObjects();
    this->updateEnemyPointer();
    this->updateRedBK();
    this->updateBar();
    this->updateEnemySerchRadius();
}

void GameManager::uptateObjects()
{
    Vec2 orgAddPos = joystick->getMoveAddPos();
    //如果英雄移动
    if (orgAddPos != Vec2::ZERO) {
        updateAddPos();
    }

    auto particles = particle_parent->getChildren();
    const auto updatePar = [&particles](UINT from, UINT to,
                                        UINT threadNumber) -> void {
        for (int x = from; x < to; ++x) {
            auto p = dynamic_cast<basic_Particle*>(particles.at(x));
            p->updateParticle();
        }
    };
    callInThread(particles.size(), updatePar);

    auto bullets = bullet_parent->getChildren();
    for (auto& it : bullets) {
        auto bullet = dynamic_cast<basic_Bullet*>(it);
        if (bullet->getTag() == tag_heroSkillBullet) {
            continue;
        }
        Vec2 bulletPos = bullet->getPosition();
        //如果距离英雄太远,kill
        if (MyMath::distance(bulletPos, hero->getPosition()) > 1200) {
            bullet->kill();
            continue;
        }
    }
}

void GameManager::updateEnemyPointer()
{
    auto enemy = this->getNearestEnemyWithLevel(hero->getPosition(),
                                                EnemySerchLevel::mid);
    //如果周围有敌人
    if (!enemy) {
        this->enemyPointer->setVisible(false);
        return;
    } else {
        this->enemyPointer->setVisible(true);
    }

    Vec2 enemyPosition = enemy->getPosition();
    if (enemyPosition != hero->getPosition()) {
        Vec2 resultPos = MyMath::getPosOnLine(
            hero->getPosition(), enemyPosition,
            max(hero->getContentSize().width, hero->getContentSize().height));
        float rotation =
            MyMath::getRotation(hero->getPosition(), enemyPosition);

        enemyPointer->setPosition(resultPos);
        enemyPointer->setRotation(rotation);
    }
}

void GameManager::updateRedBK()
{
    if (!this->redBk) {
        return;
    }

    //剩余血量百分比
    const float rate = hero->getHitPoint() / hero->getMaxHitPoint();

    //百分比低于此值会显示
    constexpr float show = 0.25f;

    if (rate < show) {
        redBk->setVisible(true);
        float percent = (1 - (rate / show) + 0.3f);
        if (percent > 1.0f) {
            percent = 1.0f;
        }
        redBk->setOpacity(percent * 255);
    } else {
        redBk->setVisible(false);
    }
}

void GameManager::updateBar()
{
    {
        char str[64] = "";
        std::snprintf(str, sizeof(str), "%d / %d", (int)hero->getHitPoint(),
                      (int)hero->getMaxHitPoint());
        bar_label[0]->setString(str);

        float r = hero->getHitPoint() / hero->getMaxHitPoint() * 100;
        bar_ui[0]->setPercent(r);
    }

    {
        char str[64] = "";
        std::snprintf(str, sizeof(str), "%d / %d", (int)hero->getEnergyPoint(),
                      (int)hero->getMaxEnergyPoint());
        bar_label[1]->setString(str);

        float r = hero->getEnergyPoint() / hero->getMaxEnergyPoint() * 100;
        bar_ui[1]->setPercent(r);
    }

    {
        char str[64] = "";
        std::snprintf(str, sizeof(str), "(%d) %d / %d",
                      enemyWaveManager.getWaveCount() + 1,
                      enemyWaveManager.getNowKilled(),
                      enemyWaveManager.getMaxKilled());
        bar_label[2]->setString(str);

        bar_ui[2]->setPercent(enemyWaveManager.getProgress() * 100);
    }
}

void GameManager::updateEnemySerchRadius()
{
    auto heroPos = hero->getPosition();
    auto enemyGroups = this->enemyGroup_parent->getChildren();
    for (auto& it : enemyGroups) {
        auto enemyGroup = dynamic_cast<EnemyGroup*>(it);
        if (MyMath::distance(heroPos, enemyGroup->getPosition()) <=
            enemyGroup->getSerchRadius()) {
            enemyGroup->setActive();
        }
    }
}

void GameManager::updateAddPos()
{
    Vec2 orgAddPos = joystick->getMoveAddPos();
    auto bullets = bullet_parent->getChildren();
    auto particles = particle_parent->getChildren();
    auto mapRect = gameMap->getBoundingBox();

    auto heroPos = hero->getPosition();
    const auto realMoveSpeed =
        hero->getMoveSpeed() / Director::getInstance()->getFrameRate();
    Vec2 add(realMoveSpeed * orgAddPos.x, realMoveSpeed * orgAddPos.y);

    //如果在边界处，移动距离大于边界，更改移动距离

    //在地图左外侧
    if (heroPos.x + -add.x < mapRect.getMinX()) {
        add.x = -(mapRect.getMinX() - heroPos.x);
    } else if (heroPos.x + -add.x > mapRect.getMaxX()) {
        //右外侧
        add.x = -(mapRect.getMaxX() - heroPos.x);
    }

    //上侧
    if (heroPos.y + -add.y > mapRect.getMaxY()) {
        add.y = -(mapRect.getMaxY() - heroPos.y);
    } else if (heroPos.y + -add.y < mapRect.getMinY()) {
        //下侧
        add.y = -(mapRect.getMinY() - heroPos.y);
    }

    gameMap->setPosition(gameMap->getPosition() + add);

    //多线程优化
    auto threadsCall = [&add](const Vector<Node*>& vec) {
        const auto f0 = [&vec, &add](UINT from, UINT to,
                                     UINT threadNumber) -> void {
            for (int x = from; x < to; ++x) {
                auto it = dynamic_cast<basic_Object*>(vec.at(x));
                it->setLastPosition(it->getLastPosition() + add);
                it->setPosition(it->getPosition() + add);
            }
        };
        callInThread(vec.size(), f0);
    };

    threadsCall(bullets);
    threadsCall(enemy_parent->getChildren());
    threadsCall(particles);

    auto enemyGroup = enemyGroup_parent->getChildren();
    for (const auto& it : enemyGroup) {
        it->setPosition(it->getPosition() + add);
    }

    auto others = others_parent->getChildren();
    for (const auto& it : others) {
        if (it->getTag() != tag_heroSkillJoystickArrow &&
            it->getTag() != tag_enemyHitPointBar) {
            it->setPosition(it->getPosition() + add);
        }
    }
}

void GameManager::setBarPosition(const Vec2& pos)
{
    this->bar_bk->setPosition(pos);

    float up = pos.y + (bar_ui[0]->getContentSize().height / 2 +
                        (bar_bk->getContentSize().height -
                         bar_ui[0]->getContentSize().height) /
                            4);
    float down = pos.y - (bar_ui[0]->getContentSize().height / 2 +
                          (bar_bk->getContentSize().height -
                           bar_ui[0]->getContentSize().height) /
                              4);

    bar_ui[0]->setPosition(Vec2(pos.x, up));
    bar_front[0]->setPosition(Vec2(pos.x, up));
    bar_label[0]->setPosition(Vec2(pos.x, up));

    bar_ui[1]->setPosition(pos);
    bar_front[1]->setPosition(pos);
    bar_label[1]->setPosition(pos);

    bar_ui[2]->setPosition(Vec2(pos.x, down));
    bar_front[2]->setPosition(Vec2(pos.x, down));
    bar_label[2]->setPosition(Vec2(pos.x, down));
}

void GameManager::createInstance()
{
    auto m = new (std::nothrow) GameManager();
    if (m && m->init()) {
        m->autorelease();
        GameManager::instance = m;
        return;
    }
    CC_SAFE_DELETE(m);
}

bool GameManager::init()
{
    if (!Node::init()) {
        return false;
    }
    this->schedule([&](float) { this->_update(); }, "__gameManager_update");

    this->particle_parent = Node::create();
    this->bullet_parent = Node::create();
    this->enemy_parent = Node::create();
    this->enemyGroup_parent = Node::create();
    this->others_parent = Node::create();
    this->skillEffect_parent = Node::create();

    particle_parent->setPosition(Vec2::ZERO);
    bullet_parent->setPosition(Vec2::ZERO);
    enemy_parent->setPosition(Vec2::ZERO);
    enemyGroup_parent->setPosition(Vec2::ZERO);
    others_parent->setPosition(Vec2::ZERO);
    skillEffect_parent->setPosition(Vec2::ZERO);

    this->addChild(particle_parent, _GameManager_zOrder::z_particle);
    this->addChild(bullet_parent, _GameManager_zOrder::z_bullet);
    this->addChild(enemy_parent, _GameManager_zOrder::z_enemy);
    this->addChild(enemyGroup_parent, _GameManager_zOrder::z_enemyGroup);
    this->addChild(others_parent, _GameManager_zOrder::z_others);
    this->addChild(skillEffect_parent, _GameManager_zOrder::z_skillEffect);

    return true;
}

void GameManager::removeFromParent()
{
    if (randMap0) {
        delete randMap0;
    }

    if (randMap1) {
        delete randMap1;
    }

    Node::removeFromParent();
}

void GameManager::addParticleChild(basic_Particle* par)
{
    this->particle_parent->addChild(par);
}

void GameManager::addBulletChild(basic_Bullet* bul)
{
    this->bullet_parent->addChild(bul);
}

void GameManager::addEnemyChild(basic_Enemy* ene)
{
    this->enemy_parent->addChild(ene);
}

void GameManager::addEnemyGroupChild(EnemyGroup* eg)
{
    this->enemyGroup_parent->addChild(eg);
}

void GameManager::addOthersChild(Node* other)
{
    this->others_parent->addChild(other);
}

void GameManager::addSkillEffectChild(Node* effect)
{
    this->skillEffect_parent->addChild(effect);
}

int GameManager::getBulletCount()
{
    return this->bullet_parent->getChildrenCount();
}

int GameManager::getEnemyCount()
{
    return this->enemy_parent->getChildrenCount();
}

int GameManager::getAttackEnemyCount()
{
    int count = 0;
    const auto pool = this->enemy_parent->getChildren();
    for (auto& it : pool) {
        auto enemy = dynamic_cast<basic_Enemy*>(it);
        if (enemy->getEnemyType() == EnemyType::attack) {
            ++count;
        }
    }
    return count;
}

int GameManager::getParticleCount()
{
    return this->particle_parent->getChildrenCount();
}

Sprite* GameManager::getHeroPauseShow() const { return heroPauseShow; }

Hero* GameManager::getHero() const { return hero; }

Joystick* GameManager::getJoystick() const { return joystick; }

void GameManager::processContact(PhysicsContact& contact)
{
    auto nodeA =
        dynamic_cast<basic_Object*>(contact.getShapeA()->getBody()->getNode());
    auto nodeB =
        dynamic_cast<basic_Object*>(contact.getShapeB()->getBody()->getNode());
    if (!nodeA || !nodeB) return;

    auto tagA = nodeA->getTag();
    auto tagB = nodeB->getTag();

    do {
        //子弹撞上英雄
        auto bulletWithHero = [&]() -> bool {
            if (tagA == tag_hero && tagB == tag_enemyBullet) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_enemyBullet && tagB == tag_hero) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        //子弹撞上敌人
        auto bulletWithEnemy = [&]() -> bool {
            if (tagA == tag_enemy && tagB == tag_heroBullet) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_heroBullet && tagB == tag_enemy) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        //经验撞到英雄
        auto expWithHero = [&]() -> bool {
            if (tagA == tag_hero && tagB == tag_exp) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_exp && tagB == tag_hero) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        //敌人子弹装上护盾
        auto bulletWithSkillShield = [&]() -> bool {
            if (tagA == tag_skillShield && tagB == tag_enemyBullet) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_enemyBullet && tagB == tag_skillShield) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        //技能子弹撞上敌人
        auto skillBulletWithEnemy = [&]() -> bool {
            if (tagA == tag_enemy && tagB == tag_heroSkillBullet) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_heroSkillBullet && tagB == tag_enemy) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        auto skillBulletWithEnemyBullet = [&]() -> bool {
            if (tagA == tag_enemyBullet && tagB == tag_heroSkillBullet) {
                nodeB->onContact(nodeA);
                return true;
            }
            if (tagA == tag_heroSkillBullet && tagB == tag_enemyBullet) {
                nodeA->onContact(nodeB);
                return true;
            }
            return false;
        };

        //先判断子弹和护盾,再判断是否和英雄
        if (bulletWithSkillShield()) break;

        if (skillBulletWithEnemy()) break;

        if (skillBulletWithEnemyBullet()) break;

        if (bulletWithHero()) break;

        if (bulletWithEnemy()) break;

        if (expWithHero()) break;

    } while (0);
}

Size GameManager::getBarContentSize() { return bar_bk->getContentSize(); }

void GameManager::nextEnemyWave(int waveNumber, int enemyNumber,
                                float hitPointAdd, float damageAdd)
{
    //显示信息
    auto act1 = FadeTo::create(0.2, 255 * 0.8);
    auto act2 = DelayTime::create(2.0);
    auto act3 = FadeOut::create(0.2);
    auto act4 = CallFuncN::create([](Node* n) { n->removeFromParent(); });

    auto action = Sequence::create(act1, act2, act3, act4, nullptr);
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    {
        auto l0 = Label::createWithTTF("", MyDefault.font.bold, 125,
                                       Size(550, 0), TextHAlignment::CENTER,
                                       TextVAlignment::CENTER);
        l0->setOpacity(0);
        l0->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 200));
        char str0[64] = "";
        std::snprintf(str0, sizeof(str0), "%s %d %s",
                      XMLManager::getString("wave0").c_str(), waveNumber + 1,
                      XMLManager::getString("wave1").c_str());
        l0->setString(str0);
        l0->runAction(action->clone());
        skillEffect_parent->addChild(l0);
    }

    {
        auto l0 = Label::createWithTTF(
            "", MyDefault.font.bold, 35, Size(visibleSize.width, 0),
            TextHAlignment::CENTER, TextVAlignment::CENTER);
        l0->setOpacity(0);
        l0->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 300));
        char str0[128] = "";
        std::snprintf(str0, sizeof(str0), "%s%d %s%.1f%% %s%.1f%%",
                      XMLManager::getString("wave2").c_str(), enemyNumber,
                      XMLManager::getString("wave3").c_str(), hitPointAdd * 100,
                      XMLManager::getString("wave4").c_str(), damageAdd * 100);
        l0->setString(str0);
        l0->runAction(action->clone());
        skillEffect_parent->addChild(l0);
    }

    //刷新敌人
    enemyCreater.createEnemies(enemyNumber, hitPointAdd, damageAdd);
}

void GameManager::startFirstEnemyWave()
{
    this->enemyWaveManager.startFirstWave();
}

GameManager* const GameManager::getInstance() { return instance; }

void GameManager::destoryInstance()
{
    if (instance->getParent()) {
        instance->removeFromParent();
    }

    instance = nullptr;
}

basic_Enemy* GameManager::getNearestEnemy(const Vec2& pos)
{
    struct {
        float distance = numeric_limits<float>::max();
        basic_Enemy* enemy = nullptr;
    } temp;

    const auto& pool = this->enemy_parent->getChildren();

    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto _enemy = dynamic_cast<basic_Enemy*>(*it);
        if (_enemy->getEnemyType() == EnemyType::cannotSerch) {
            continue;
        }

        float distance = MyMath::distance(_enemy->getPosition(),
                                          pos);  //敌人与英雄之间的距离
        if (temp.distance > distance) {
            temp.distance = distance;
            temp.enemy = _enemy;
        }
    }

    return temp.enemy;
}

basic_Enemy* GameManager::getNearestEnemyWithLevel(const Vec2& pos,
                                                   EnemySerchLevel level)
{
    struct {
        float distance = numeric_limits<float>::max();
        basic_Enemy* enemy = nullptr;
    } temp;

    const auto& pool = this->enemy_parent->getChildren();
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto _enemy = dynamic_cast<basic_Enemy*>(*it);
        if (_enemy->getEnemyType() == EnemyType::cannotSerch ||
            _enemy->getEnemySerchLevel() != level) {
            continue;
        }

        float distance = MyMath::distance(_enemy->getPosition(),
                                          pos);  //敌人与英雄之间的距离
        if (temp.distance > distance) {
            temp.distance = distance;
            temp.enemy = _enemy;
        }
    }

    return temp.enemy;
}

basic_Enemy* GameManager::getNearestEnemyWithLevel(const Vec2& pos, int range)
{
    /*
    搜索目标：
    先按等级划分,如果在 range 内 取更高级的
    */
    struct {
        float distance = numeric_limits<float>::max();
        basic_Enemy* enemy = nullptr;
    } temp;

    const auto& pool = this->enemy_parent->getChildren();

    //当前最高的搜索等级
    EnemySerchLevel highestSerchLevel = EnemySerchLevel::unknow;
    bool findAttackEnemy = false;  //是否搜寻到正在攻击的敌人

    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto _enemy = dynamic_cast<basic_Enemy*>(*it);
        auto type = _enemy->getEnemyType();
        auto _serchLevel = _enemy->getEnemySerchLevel();
        auto distance = MyMath::distance(_enemy->getPosition(), pos);

        if (distance > range || type == EnemyType::cannotSerch ||
            (findAttackEnemy == true && type != EnemyType::attack)) {
            continue;
        }

        if (type == EnemyType::attack) {
            findAttackEnemy = true;
        }

        //从高到低排,如果当前敌人的等级低于最高等级,不搜索
        if (_serchLevel >= highestSerchLevel) {
            highestSerchLevel = _serchLevel;
        } else {
            continue;
        }

        if (temp.distance > distance) {
            temp.distance = distance;
            temp.enemy = _enemy;
        }
    }

    return temp.enemy;
}

basic_Enemy* GameManager::getNearestEnemyWithLevel(const Vec2& pos)
{
    return this->getNearestEnemyWithLevel(
        pos, INT_MAX - 1 /*不传入距离默认最大距离*/);
}

vector<basic_Enemy*> GameManager::getEnemyInRange(const Vec2& pos, float range)
{
    vector<basic_Enemy*> temp;
    auto pool = enemy_parent->getChildren();
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto _enemy = dynamic_cast<basic_Enemy*>(*it);
        if (_enemy->getEnemyType() == EnemyType::attack ||
            _enemy->getEnemyType() == EnemyType::canSerch) {
            if (MyMath::distance(pos, _enemy->getPosition()) < range) {
                temp.push_back(_enemy);
            }
        }
    }
    return temp;
}

vector<basic_Bullet*> GameManager::getEnemyBulletInRange(const Vec2& pos,
                                                         int range)
{
    vector<basic_Bullet*> temp;
    auto pool = bullet_parent->getChildren();
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto bullet = dynamic_cast<basic_Bullet*>(*it);
        if (bullet->getTag() == tag_enemyBullet) {
            if (MyMath::distance(pos, bullet->getPosition()) < range) {
                temp.push_back(bullet);
            }
        }
    }
    return temp;
}

Vec2 GameManager::randomPosInMap()
{
    Vec2 mapPos = gameMap->getPosition();

    const auto px = (*randMap0)();
    const auto py = (*randMap1)();

    return Vec2(px, py) + mapPos;
}

Vec2 GameManager::randomPosInMap(const Vec2& pos, float r)
{
    while (1) {
        auto p1 = this->randomPosInMap();
        //要在给定的圆之外
        if (MyMath::distance(pos, p1) >= r) {
            return p1;
        }
    }
}

void GameManager::enemyDie()
{
    MyDefault.gameState.enemyKilled += 1;
    enemyWaveManager.enemyDie();
}

void GameManager::onHeroDie()
{
    switch_heroIsDeid.setSwitch(true);

    hero->setVisible(false);
    hero->getCannon()->setVisible(false);

    auto particleOnExplode = [&]() -> void {
        rand_int r0(0,
                    heroExplode.size() - 1);  //范围可能改变,不能设置成 static
        auto particle = ParticleOnExplode::create(
            heroExplode[r0()],
            MyDefault.heroExplode.particleLiveTime_OnExplode);
        if (particle) {
            particle->setPosition(hero->getPosition());
            GameManager::getInstance()->addParticleChild(particle);
        }
    };

    auto v = static_cast<unsigned int>(MyMath::floatToInt(
        (float)MyDefault.heroExplode.particleQuantity_OnExplode *
        ((MyDefault.option.particleQuality + 1) / 3.0f)));
    for (size_t x = 0; x < v; ++x) {
        particleOnExplode();
    }
}

void EnemyWaveManager::enemyDie()
{
    const int maxEnemyCount = MyDefault.enemyWave.baseEnemyNumbers + waveCount;

    //下一波
    if (nowEnemyKilled + 1 == maxEnemyCount) {
        ++waveCount;
        nowEnemyKilled = 0;
        GameManager::getInstance()->nextEnemyWave(
            waveCount, MyDefault.enemyWave.baseEnemyNumbers + waveCount,
            MyDefault.enemyWave.hitPointAdd * waveCount,
            MyDefault.enemyWave.attackDamageAdd * waveCount);
    } else {
        ++nowEnemyKilled;
    }
}

void EnemyWaveManager::startFirstWave()
{
    GameManager::getInstance()->nextEnemyWave(
        waveCount, MyDefault.enemyWave.baseEnemyNumbers + waveCount,
        MyDefault.enemyWave.hitPointAdd * waveCount,
        MyDefault.enemyWave.attackDamageAdd * waveCount);
}

float EnemyWaveManager::getProgress()
{
    return static_cast<float>(nowEnemyKilled) /
           static_cast<float>(MyDefault.enemyWave.baseEnemyNumbers + waveCount);
}

int EnemyWaveManager::getNowKilled() { return nowEnemyKilled; }

int EnemyWaveManager::getMaxKilled()
{
    return MyDefault.enemyWave.baseEnemyNumbers + waveCount;
}

int EnemyWaveManager::getWaveCount() { return waveCount; }

void EnemyCreater::createEnemies(int enemyNumber, float hitPointAdd,
                                 float damageAdd)
{
    auto gameManager = GameManager::getInstance();
    auto heroPos = gameManager->getHero()->getPosition();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    for (int x = 0; x < enemyNumber;) {
        auto enemy_weapon0 = Weapon_Enemy01::create();
        enemy_weapon0->setBulletInfo(
            Imgs.onGame.enemyBullet1.p_bullet,
            Imgs.onGame.enemyBullet1.vp_explode,
            Imgs.onGame.enemyBullet1.vp_move, "enemy_bullet_02",
            MyDefault.enemy02_bullet.moveSpeed,
            MyDefault.enemy02_bullet.particleLiveTime_OnMove,
            MyDefault.enemy02_bullet.particleCreateSpeed_OnMove,
            MyDefault.enemy02_bullet.particleLiveTime_OnExplode,
            MyDefault.enemy02_bullet.particleQuantity_OnExplode,
            MyDefault.enemy02_bullet.damage * (1 + damageAdd));

        auto enemy_weapon1 = Weapon_Enemy00::create();
        enemy_weapon1->setBulletInfo(
            Imgs.onGame.enemyBullet0.p_bullet,
            Imgs.onGame.enemyBullet0.vp_explode,
            Imgs.onGame.enemyBullet0.vp_move, "enemy_bullet",
            MyDefault.enemy01_bullet.moveSpeed,
            MyDefault.enemy01_bullet.particleLiveTime_OnMove,
            MyDefault.enemy01_bullet.particleCreateSpeed_OnMove,
            MyDefault.enemy01_bullet.particleLiveTime_OnExplode,
            MyDefault.enemy01_bullet.particleQuantity_OnExplode,
            MyDefault.enemy01_bullet.damage * (1 + damageAdd));

        auto pos = gameManager->randomPosInMap(
            heroPos, max(visibleSize.width, visibleSize.height) / 1.8);

        auto enemyGroup = EnemyGroup::create(Imgs.onGame.enemyGroup.p_group);
        enemyGroup->setLineFrame(Imgs.onGame.enemyGroup.p_lineDot);
        enemyGroup->setCircleFrame(Imgs.onGame.enemyGroup.p_circle);
        enemyGroup->setLineIntervalDistance(75);
        enemyGroup->setPosition(pos);
        enemyGroup->setSerchRadius(450);

        gameManager->addEnemyGroupChild(enemyGroup);

        rand_int r0(80, 120);
        rand_int r1(150, 200);
        rand_int r2(1, 100);
        rand_bool r3;
        int rate = r2();

        int number = 0;
        if (rate <= 10) {
            number = 2;
        } else if (rate <= 30) {
            number = 3;
        } else if (rate <= 70) {
            number = 4;
        } else if (rate <= 100) {
            number = 5;
        }

        if (number + x > enemyNumber) {
            number = enemyNumber - x;
        }
        x += number;

        static map<ExpType, string> expFrames{
            {ExpType::HitPoint, Imgs.onGame.exp.p_hp},
            {ExpType::EnergyPoint, Imgs.onGame.exp.p_ep}};

        for (int x = 0; x < number; ++x) {
            if (r3()) {
                auto enemy1 =
                    EnemyWithAI0::create(Imgs.onGame.enemy0.p_enemy, "enemy");
                GameManager::getInstance()->addEnemyChild(enemy1);
                enemy1->setEnemy(MyDefault.enemy01.hitPoint * (1 + hitPointAdd),
                                 Imgs.onGame.enemy0.vp_explode,
                                 MyDefault.enemy01.particleLiveTime_OnExplode,
                                 MyDefault.enemy01.particleQuantity_OnExplode);
                enemy1->setExp(expFrames, 10 * (1 + hitPointAdd / 1.5),
                               30 * (1 + hitPointAdd), "exp");
                enemy1->setWeaponAndAddChild(enemy_weapon1->clone());
                enemy1->setPosition(MyMath::randPos(pos, r0));
                enemy1->setSerchRadius(250.0f);

                enemyGroup->addEnemyToGroup(enemy1);
            } else {
                auto enemy = EnemyWithAI1::create(Imgs.onGame.enemy1.p_enemy,
                                                  "enemy_02");
                GameManager::getInstance()->addEnemyChild(enemy);
                enemy->setEnemy(MyDefault.enemy02.hitPoint * (1 + hitPointAdd),
                                Imgs.onGame.enemy1.vp_explode,
                                MyDefault.enemy02.particleLiveTime_OnExplode,
                                MyDefault.enemy02.particleQuantity_OnExplode);
                enemy->setExp(expFrames, 10 * (1 + hitPointAdd / 1.5),
                              30 * (1 + hitPointAdd), "exp");
                enemy->setWeapon(enemy_weapon0->clone());
                enemy->setPosition(MyMath::randPos(pos, r0));
                enemy->setSerchRadius(250.0f);

                enemyGroup->addEnemyToGroup(enemy);
            }
        }

        enemyGroup->refreshLine();
    }
}
