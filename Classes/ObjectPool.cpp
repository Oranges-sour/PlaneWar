#include "header/ObjectPool.h"

#include <map>
using namespace std;

#include "header/GameManager.h"
#include "header/Joystick.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"

constexpr float MAX_DIS = 1e9;

Touch* TouchesPool::getNearest(const Vec2& pos)
{
    struct {
        float distance = MAX_DIS;
        Touch* touch = nullptr;
    } temp;

    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto touch = *it;
        float _distance = MyMath::distance(touch->getLocation(), pos);
        if (_distance < temp.distance) {
            temp.distance = _distance;
            temp.touch = touch;
        }
    }
    return temp.touch;
}

Touch* TouchesPool::getNearest_start(const Vec2& pos)
{
    struct {
        float distance = MAX_DIS;
        Touch* touch = nullptr;
    } temp;

    for (auto it = pool.begin(); it != pool.end(); ++it) {
        auto touch = *it;
        float _distance = MyMath::distance(touch->getStartLocation(), pos);
        if (_distance < temp.distance) {
            temp.distance = _distance;
            temp.touch = touch;
        }
    }
    return temp.touch;
}

void TouchesPool::setJoystickMoveCatched(Touch* touch)
{
    this->joystick_move_catched = touch;
}

void TouchesPool::setJoystickSkillCatched(Touch* touch)
{
    this->joystick_skill_catched = touch;
}

void TouchesPool::setJoystickAttackCatched(Touch* touch)
{
    this->joystick_attack_catched = touch;
}

void TouchesPool::removeFromPool(Touch* obj)
{
    if (MyDefault.gameState.isOnGame == true) {
        if (this->joystick_move_catched == obj) {
            GameManager::getInstance()->getJoystick()->removeMoveTouchCatched();
            joystick_move_catched = nullptr;
        }
        if (this->joystick_skill_catched == obj) {
            GameManager::getInstance()
                ->getJoystick()
                ->removeSkillTouchCatched();
            joystick_skill_catched = nullptr;
        }
        if (this->joystick_attack_catched == obj) {
            GameManager::getInstance()
                ->getJoystick()
                ->removeAttackTouchCatched();
            joystick_attack_catched = nullptr;
        }
    }

    base_Pool::removeFromPool(obj);
}

void TouchesPool::removeAllObjects()
{
    if (MyDefault.gameState.isOnGame == true) {
        GameManager::getInstance()->getJoystick()->removeSkillTouchCatched();
        GameManager::getInstance()->getJoystick()->removeMoveTouchCatched();
    }
    pool.clear();
}

TouchesPool* TouchesPool::instance = new TouchesPool();
