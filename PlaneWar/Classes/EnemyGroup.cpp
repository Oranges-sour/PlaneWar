#include "header/EnemyGroup.h"

#include <cstdlib>
#include <sstream>

#include "header/Enemy.h"
#include "header/GameManager.h"
#include "header/Hero.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"

EnemyGroup* EnemyGroup::create(const string& frame)
{
    auto ep = new (std::nothrow) EnemyGroup();
    if (ep && ep->initWithSpriteFrameName(frame)) {
        ep->numberOfEnemy = Label::createWithTTF(
            "", MyDefault.font.medium, 35, Size(250, 0), TextHAlignment::CENTER,
            TextVAlignment::CENTER);
        ep->setColor(Color3B(200, 200, 200));
        //显示在所有精灵上方
        GameManager::getInstance()->addSkillEffectChild(ep->numberOfEnemy);
        ep->setTag(tag_enemyGroup);
        ep->init();

        ep->autorelease();
        return ep;
    }
    CC_SAFE_DELETE(ep);
    return nullptr;
}

void EnemyGroup::setLineFrame(const string& frame) { this->lineFrame = frame; }

void EnemyGroup::setCircleFrame(const string& frame) { circleFrame = frame; }

void EnemyGroup::setLineIntervalDistance(float distance)
{
    this->lineIntervalDistance = distance;
}

void EnemyGroup::addEnemyToGroup(base_attackEnemy* enemy)
{
    this->enemyPool.push_back(enemy);
    enemy->setIsInEnemyGroup(true);
}

float EnemyGroup::getSerchRadius() { return this->serchRadius; }

void EnemyGroup::setSerchRadius(float radius) { this->serchRadius = radius; }

void EnemyGroup::setActive()
{
    for (auto& it : enemyPool) {
        it->startAttack();
    }

    this->removeFromParent();
}

void EnemyGroup::refreshLine()
{
    //先清除原来的线
    for (auto& it : lineDots) {
        if (it) {
            it->removeFromParent();
        }
    }
    lineDots.clear();

    //先清除原来的圆圈
    for (auto& it : circle) {
        if (it) {
            it->removeFromParent();
        }
    }
    circle.clear();

    //根据敌人位置添加线
    for (auto& it : enemyPool) {
        auto pos0 = this->getPosition();
        auto pos1 = it->getPosition();
        const float number =
            MyMath::distance(pos0, pos1) / this->lineIntervalDistance;
        const float rotation = MyMath::getRotation(pos0, pos1);
        if (number >= 1.0f) {
            const int intNumber = static_cast<int>(number);
            for (int x = 0; x < intNumber; ++x) {
                auto lineDot =
                    Sprite::createWithSpriteFrameName(this->lineFrame);
                auto dotPos = MyMath::getPosOnLine(
                    pos0, pos1, (x + 1) * this->lineIntervalDistance);
                lineDot->setPosition(dotPos);
                lineDot->setRotation(rotation);
                GameManager::getInstance()->addOthersChild(lineDot);
                this->lineDots.push_back(lineDot);
            }
        } else {
            auto lineDot = Sprite::createWithSpriteFrameName(this->lineFrame);
            auto dotPos = MyMath::getPosOnLine(
                pos0, pos1, MyMath::distance(pos0, pos1) / 2.0f);
            lineDot->setPosition(dotPos);
            lineDot->setRotation(rotation);
            GameManager::getInstance()->addOthersChild(lineDot);
            this->lineDots.push_back(lineDot);
        }
    }
    //根据半径添加圆圈
    for (float angle = 0; angle < 179; angle += 10) {
        auto sp0 = Sprite::createWithSpriteFrameName(circleFrame);
        auto sp1 = Sprite::createWithSpriteFrameName(circleFrame);
        GameManager::getInstance()->addOthersChild(sp0);
        GameManager::getInstance()->addOthersChild(sp1);
        this->circle.push_back(sp0);
        this->circle.push_back(sp1);
    }

    numberOfEnemy->setPosition(
        Vec2(this->getPosition().x,
             this->getPosition().y + this->getContentSize().height / 1.5f));
    stringstream ss;
    ss << "x" << enemyPool.size();
    string temp;
    ss >> temp;
    numberOfEnemy->setString(temp);
}

void EnemyGroup::updateCircle()
{
    int cnt = 0;
    for (float angle = 0; angle < 179 && cnt < circle.size(); angle += 10) {
        auto thisPos = this->getPosition();
        const float k = DEG::tan(angle);
        const float x0 = serchRadius / sqrt(POT(k) + 1);
        const float x1 = -x0;
        const float y0 = x0 * k;
        const float y1 = x1 * k;

        Vec2 p0 = thisPos + Vec2(x0, y0);
        Vec2 p1 = thisPos + Vec2(x1, y1);

        auto sp0 = circle[cnt++];
        auto sp1 = circle[cnt++];
        sp0->setPosition(p0);
        sp0->setRotation(MyMath::getRotation(thisPos, p0));
        sp1->setPosition(p1);
        sp1->setRotation(MyMath::getRotation(thisPos, p1));
    }
}

void EnemyGroup::removeFromParent()
{
    for (auto& it : lineDots) {
        it->removeFromParent();
    }
    for (auto& it : circle) {
        it->removeFromParent();
    }
    numberOfEnemy->removeFromParent();
    Sprite::removeFromParent();
}

bool EnemyGroup::init()
{
    this->schedule([&](float) { this->_update(); }, "update");
    return true;
}

void EnemyGroup::_update()
{
    auto nowPos = this->getPosition();
    auto heroPos = GameManager::getInstance()->getHero()->getPosition();
    auto labelPos = MyMath::getPosOnLine(nowPos, heroPos, 75);

    this->numberOfEnemy->setPosition(labelPos);

    if (serchRadius < 1000) {
        const float frameRate = Director::getInstance()->getFrameRate();
        this->serchRadius += 30.0f / frameRate;
        updateCircle();
    }
}
