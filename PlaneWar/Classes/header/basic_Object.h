#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "cocos2d.h"
USING_NS_CC;

class basic_Object : public Sprite {
public:
    virtual void kill() { this->removeFromParent(); }
    virtual Vec2 getLastPosition() { return Vec2(); }
    virtual void setLastPosition(const Vec2& pos) {}

    //均为撞上别人,而不是被撞
    virtual void onContact(Node* target) {}

    virtual void getAttack(float damage) {}

    virtual void _update() {}
};

#endif