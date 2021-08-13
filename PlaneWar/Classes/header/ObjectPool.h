#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <list>
#include <map>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

template <class T>
class base_Pool {
public:
    virtual list<T*> getAllObjects();
    virtual bool addToPool(T* obj);
    virtual void removeFromPool(T* obj);
    virtual void removeAllObjects();
    virtual unsigned int getUsefulObjects();

protected:
    base_Pool() {}
    ~base_Pool() {}
    list<T*> pool;
};

template <class T>
inline unsigned int base_Pool<T>::getUsefulObjects()
{
    return pool.size();
}

template <class T>
inline list<T*> base_Pool<T>::getAllObjects()
{
    return pool;
}

template <class T>
inline bool base_Pool<T>::addToPool(T* obj)
{
    pool.push_front(obj);
    return true;
}

template <class T>
inline void base_Pool<T>::removeFromPool(T* obj)
{
    pool.remove(obj);
}

template <class T>
inline void base_Pool<T>::removeAllObjects()
{
    pool.clear();
}

class TouchesPool : public base_Pool<Touch> {
public:
    static TouchesPool* getInstance() { return instance; }

private:
    TouchesPool() {}
    ~TouchesPool() {}
    static TouchesPool* instance;

public:
    //获得屏幕最近的触摸
    Touch* getNearest(const Vec2& pos);

    //获得触控起始点最近
    Touch* getNearest_start(const Vec2& pos);

    //除了class joystick::move  外其他类不得调用
    void setJoystickMoveCatched(Touch* touch);

    void setJoystickSkillCatched(Touch* touch);

    void setJoystickAttackCatched(Touch* touch);

    virtual void removeFromPool(Touch* obj) override;
    virtual void removeAllObjects() override;

private:
    //移动触控杆 追踪的touch
    Touch* joystick_move_catched = nullptr;

    Touch* joystick_skill_catched = nullptr;

    Touch* joystick_attack_catched = nullptr;
};

#endif
