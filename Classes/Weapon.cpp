#include "header/Weapon.h"

#include "header/Bullet.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/Joystick.h"
#include "header/MyMath.h"
#include "header/PhysicsShapeCache.h"

Weapon_Hero01* Weapon_Hero01::create()
{
    auto w = new (std::nothrow) Weapon_Hero01();
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return nullptr;
}

Vec2 Weapon_Hero01::attack(const Vec2& pos)
{
    auto hero = GameManager::getInstance()->getHero();
    if (hero->getEnergyPoint() >= energy) {
        //如果能量值小于一次发射所需能量就不发射
        hero->setEnergyPoint(hero->getEnergyPoint() - energy);
        auto bullet = BulletForHero_01::create(bulletFrame, particleOnExplode,
                                               particleOnMove);
        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, bullet);
        bullet->setTag(tag_heroBullet);
        GameManager::getInstance()->addBulletChild(bullet);

        //返回值为射击点坐标
        return bullet->shoot(pos, moveSpeed, particleLiveTime_OnMove,
                             particleCreateSpeed_OnMove,
                             particleLiveTime_OnExplode,
                             particleQuantity_OnExplode, range, damage);
    }
    return Vec2();
}

Weapon_Hero01* Weapon_Hero01::clone()
{
    auto result = Weapon_Hero01::create();
    result->setBulletInfo(
        bulletFrame, particleOnExplode, particleOnMove, physicsInfo, moveSpeed,
        particleLiveTime_OnMove, particleCreateSpeed_OnMove,
        particleLiveTime_OnExplode, particleQuantity_OnExplode, damage, energy,
        range, attackInterval);
    return result;
}

void Weapon_Hero01::setBulletInfo(
    const string& bullet, const vector<string>& particleOnExplode,
    const vector<string>& particleOnMove, const string& physicsInfo,
    float moveSpeed, float particleLiveTime_OnMove,
    float particleCreateSpeed_OnMove, float particleLiveTime_OnExplode,
    unsigned int particleQuantity_OnExplode, float damage, int energy,
    int range, float attackInterval)
{
    this->bulletFrame = bullet;
    this->particleOnExplode = particleOnExplode;
    this->particleOnMove = particleOnMove;
    this->physicsInfo = physicsInfo;
    this->moveSpeed = moveSpeed;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleCreateSpeed_OnMove = particleCreateSpeed_OnMove;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->damage = damage;
    this->energy = energy;
    this->range = range;
    this->attackInterval = attackInterval;
}

float Weapon_Hero01::getAttackInterval() { return attackInterval; }

//////////////////////////////////////////////////////////////////////////

Weapon_Enemy00* Weapon_Enemy00::create()
{
    auto w = new (std::nothrow) Weapon_Enemy00();
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return nullptr;
}

Vec2 Weapon_Enemy00::attack(const Vec2& from)
{
    auto bullet =
        BulletForEnemy0::create(bulletFrame, particleOnExplode, particleOnMove);
    PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, bullet);
    bullet->setTag(tag_enemyBullet);
    GameManager::getInstance()->addBulletChild(bullet);

    //返回值为射击点坐标
    return bullet->shoot(from, moveSpeed, particleLiveTime_OnMove,
                         particleCreateSpeed_OnMove, particleLiveTime_OnExplode,
                         particleQuantity_OnExplode, damage);
}

bool Weapon_Enemy00::init() { return true; }

Weapon_Enemy00* Weapon_Enemy00::clone()
{
    auto result = Weapon_Enemy00::create();
    result->setBulletInfo(
        bulletFrame, particleOnExplode, particleOnMove, physicsInfo, moveSpeed,
        particleLiveTime_OnMove, particleCreateSpeed_OnMove,
        particleLiveTime_OnExplode, particleQuantity_OnExplode, damage);
    return result;
}

void Weapon_Enemy00::setBulletInfo(
    const string& bullet, const vector<string>& particleOnExplode,
    const vector<string>& particleOnMove, const string& physicsInfo,
    float moveSpeed, float particleLiveTime_OnMove,
    float particleCreateSpeed_OnMove, float particleLiveTime_OnExplode,
    unsigned int particleQuantity_OnExplode, float damage)
{
    this->bulletFrame = bullet;
    this->particleOnExplode = particleOnExplode;
    this->particleOnMove = particleOnMove;
    this->physicsInfo = physicsInfo;
    this->moveSpeed = moveSpeed;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleCreateSpeed_OnMove = particleCreateSpeed_OnMove;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->damage = damage;
}

float Weapon_Enemy00::getAttackInterval() { return -1.0f; }

//////////////////////////////////////////////////////////////////////////

Weapon_Enemy01* Weapon_Enemy01::create()
{
    auto w = new (std::nothrow) Weapon_Enemy01();
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return nullptr;
}

Vec2 Weapon_Enemy01::attack(const Vec2& from, float angle)
{
    auto bullet =
        BulletForEnemy1::create(bulletFrame, particleOnExplode, particleOnMove);
    PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, bullet);
    bullet->setTag(tag_enemyBullet);
    GameManager::getInstance()->addBulletChild(bullet);

    //返回值为射击点坐标
    return bullet->shoot(from, moveSpeed, particleLiveTime_OnMove,
                         particleCreateSpeed_OnMove, particleLiveTime_OnExplode,
                         particleQuantity_OnExplode, damage, angle);

    return Vec2();
}

bool Weapon_Enemy01::init() { return true; }

Weapon_Enemy01* Weapon_Enemy01::clone()
{
    auto result = Weapon_Enemy01::create();
    result->setBulletInfo(
        bulletFrame, particleOnExplode, particleOnMove, physicsInfo, moveSpeed,
        particleLiveTime_OnMove, particleCreateSpeed_OnMove,
        particleLiveTime_OnExplode, particleQuantity_OnExplode, damage);
    return result;
}

void Weapon_Enemy01::setBulletInfo(
    const string& bullet, const vector<string>& particleOnExplode,
    const vector<string>& particleOnMove, const string& physicsInfo,
    float moveSpeed, float particleLiveTime_OnMove,
    float particleCreateSpeed_OnMove, float particleLiveTime_OnExplode,
    unsigned int particleQuantity_OnExplode, float damage)
{
    this->bulletFrame = bullet;
    this->particleOnExplode = particleOnExplode;
    this->particleOnMove = particleOnMove;
    this->physicsInfo = physicsInfo;
    this->moveSpeed = moveSpeed;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleCreateSpeed_OnMove = particleCreateSpeed_OnMove;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->damage = damage;
}

float Weapon_Enemy01::getAttackInterval() { return -1.0f; }

//////////////////////////////////////////////////////////////////////////
#if 1

//废弃的

#else

Weapon_HeroSkill* Weapon_HeroSkill::create()
{
    auto w = new (std::nothrow) Weapon_HeroSkill();
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return nullptr;
}

Vec2 Weapon_HeroSkill::attack(const Vec2&)
{
    auto bullet = BulletForHeroSkill::create(bulletFrame, particleOnExplode,
                                             particleOnMove);
    PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, bullet);
    bullet->setTag(tag_heroSkillBullet);
    GameManager::getInstance()->addBulletChild(bullet);

    auto heroPos = GameManager::getInstance()->getHero()->getPosition();

    auto toPos = [&]() -> Vec2 { return MyMath::randPos(heroPos, 150, 400); };

    //返回值为射击点坐标
    return bullet->shoot(heroPos, toPos(), particleLiveTime_OnMove,
                         particleCreateSpeed_OnMove, particleLiveTime_OnExplode,
                         particleQuantity_OnExplode, moveSpeed, range, damage);

    return Vec2();
}

bool Weapon_HeroSkill::init() { return true; }

Weapon_HeroSkill* Weapon_HeroSkill::clone()
{
    auto result = Weapon_HeroSkill::create();
    result->setBulletInfo(
        bulletFrame, particleOnExplode, particleOnMove, physicsInfo, moveSpeed,
        particleLiveTime_OnMove, particleCreateSpeed_OnMove,
        particleLiveTime_OnExplode, particleQuantity_OnExplode, range, damage);
    return result;
}

void Weapon_HeroSkill::setBulletInfo(
    const string& bullet,                     //子弹精灵帧
    const vector<string>& particleOnExplode,  //爆炸粒子
    const vector<string>& particleOnMove,     //移动粒子
    const string& physicsInfo,
    float moveSpeed,                //移动速度,每秒
    float particleLiveTime_OnMove,  //移动粒子生存时间(小范围随机)
    float particleCreateSpeed_OnMove,  //移动粒子产生速度,每秒
    float particleLiveTime_OnExplode,  //爆炸粒子生存时间(小范围随机)
    unsigned int particleQuantity_OnExplode,  //爆炸粒子数量
    int range,                                //追踪范围
    float damage                              //伤害
)
{
    this->bulletFrame = bullet;
    this->particleOnExplode = particleOnExplode;
    this->particleOnMove = particleOnMove;
    this->physicsInfo = physicsInfo;
    this->moveSpeed = moveSpeed;

    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleCreateSpeed_OnMove = particleCreateSpeed_OnMove;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->range = range;
    this->damage = damage;
}

float Weapon_HeroSkill::getAttackInterval() { return -1; }

#endif

//////////////////////////////////////////////////////////////////////////

Weapon_Hero02* Weapon_Hero02::create()
{
    auto w = new (std::nothrow) Weapon_Hero02();
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return nullptr;
}

Vec2 Weapon_Hero02::attack(const Vec2& from)
{
    auto hero = GameManager::getInstance()->getHero();
    if (hero->getEnergyPoint() >= energy) {
        //如果能量值小于一次发射所需能量就不发射
        hero->setEnergyPoint(hero->getEnergyPoint() - energy);
        auto bullet = BulletForHero_02::create(bulletFrame, particleOnExplode,
                                               particleOnMove);
        PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo, bullet);
        bullet->setTag(tag_heroBullet);
        GameManager::getInstance()->addBulletChild(bullet);
        auto dPos =
            GameManager::getInstance()->getJoystick()->getAttackAddPos();
        float angle = MyMath::getRotation(from, from + dPos);
        //返回值为射击点坐标
        return bullet->shoot(
            from, moveSpeed, particleLiveTime_OnMove,
            particleCreateSpeed_OnMove, particleLiveTime_OnExplode,
            particleQuantity_OnExplode, range, damage, angle, angleRotateSpeed);
    }
    return Vec2();
}

Weapon_Hero02* Weapon_Hero02::clone()
{
    auto result = Weapon_Hero02::create();
    result->setBulletInfo(
        bulletFrame, particleOnExplode, particleOnMove, physicsInfo, moveSpeed,
        particleLiveTime_OnMove, particleCreateSpeed_OnMove,
        particleLiveTime_OnExplode, particleQuantity_OnExplode, damage, energy,
        range, angleRotateSpeed, attackInterval);
    return result;
}

void Weapon_Hero02::setBulletInfo(
    const string& bullet, const vector<string>& particleOnExplode,
    const vector<string>& particleOnMove, const string& physicsInfo,
    float moveSpeed, float particleLiveTime_OnMove,
    float particleCreateSpeed_OnMove, float particleLiveTime_OnExplode,
    unsigned int particleQuantity_OnExplode, float damage, int energy,
    int range, float angleRotateSpeed, float attackInterval)
{
    this->bulletFrame = bullet;
    this->particleOnExplode = particleOnExplode;
    this->particleOnMove = particleOnMove;
    this->physicsInfo = physicsInfo;
    this->moveSpeed = moveSpeed;
    this->particleLiveTime_OnMove = particleLiveTime_OnMove;
    this->particleCreateSpeed_OnMove = particleCreateSpeed_OnMove;
    this->particleLiveTime_OnExplode = particleLiveTime_OnExplode;
    this->particleQuantity_OnExplode = particleQuantity_OnExplode;
    this->damage = damage;
    this->energy = energy;
    this->range = range;
    this->angleRotateSpeed = angleRotateSpeed;
    this->attackInterval = attackInterval;
}

float Weapon_Hero02::getAttackInterval() { return this->attackInterval; }
