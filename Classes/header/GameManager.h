#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <array>
#include <string>
using namespace std;

#include "ui/UILoadingBar.h"
using namespace cocos2d::ui;
#include "cocos2d.h"
USING_NS_CC;

#include "Enemy.h"
#include "Random.h"
#include "Switch.h"

class Hero;
class Joystick;
class basic_Particle;
class basic_Bullet;
class basic_Enemy;
class EnemyGroup;

enum _Objects_Tag {
    tag_hero = 1000,
    tag_enemyBullet = 1001,
    tag_enemy = 1002,
    tag_heroBullet = 1003,
    tag_exp = 1004,
    tag_enemyGroup = 1005,
    tag_skillShield = 1006,
    tag_skillShieldEffect = 1007,
    tag_heroSkillBullet = 1008,
    tag_heroSkillJoystickArrow = 1009,
    tag_enemyHitPointBar = 1010
};

class EnemyWaveManager {
public:
    //敌人死亡
    void enemyDie();

    void startFirstWave();
    float getProgress();
    int getNowKilled();
    int getMaxKilled();
    int getWaveCount();

private:
    int waveCount = 0;
    int nowEnemyKilled = 0;
};

class EnemyCreater {
public:
    void createEnemies(int enemyNumber, float hitPointAdd, float damageAdd);
};

class GameManager final : public Node {
public:
    static void createInstance();
    static GameManager* const getInstance();
    static void destoryInstance();
    GameManager();
    virtual ~GameManager();

private:
    static GameManager* instance;

public:
    //创建游戏背景
    bool createGameBkground(const string& bk_texture);

    //创建游戏地图
    bool createGameMap(const string& map_texture);

    //创建控制杆
    bool createJoystick(const vector<string>& move,
                        const vector<string>& attack,
                        const vector<string>& skill);

    //创建英雄
    bool createHero(const string& hero_plist, const string& cannon_plist,
                    const vector<string>& hero_explode_plist,
                    const string& physicsInfo);

    //创建敌人指示器
    bool createEnemyPointer(const string& pointer_plist);

    //创建红血背景
    bool createRedBK(const string& bk_texture);

    //创建血条框
    bool createBar(const string& bar_bk, const string& bar_front,
                   const string& hp, const string& ep, const string& progress,
                   const string& font);

public:
    Sprite* getHeroPauseShow() const;
    Hero* getHero() const;
    Joystick* getJoystick() const;
    void processContact(PhysicsContact& contact);
    Size getBarContentSize();
    void nextEnemyWave(int waveNumber, int enemyNumber, float hitPointAdd,
                       float damageAdd);
    void startFirstEnemyWave();

public:
    void addParticleChild(basic_Particle* par);
    void addBulletChild(basic_Bullet* bul);
    void addEnemyChild(basic_Enemy* ene);
    void addEnemyGroupChild(EnemyGroup* eg);
    void addOthersChild(Node* other);
    void addSkillEffectChild(Node* effect);

public:
    int getBulletCount();
    int getEnemyCount();
    int getAttackEnemyCount();
    int getParticleCount();

public:
    //获得最近的敌人
    basic_Enemy* getNearestEnemy(const Vec2& pos);

    basic_Enemy* getNearestEnemyWithLevel(const Vec2& pos,
                                          EnemySerchLevel level);

    basic_Enemy* getNearestEnemyWithLevel(const Vec2& pos, int range);
    basic_Enemy* getNearestEnemyWithLevel(const Vec2& pos);

    vector<basic_Enemy*> getEnemyInRange(const Vec2& pos, float range);
    vector<basic_Bullet*> getEnemyBulletInRange(const Vec2& pos, int range);

    //在地图上随机一个坐标
    Vec2 randomPosInMap();

    //在地图上随机一个坐标,在所给的点的半径范围外,如果半径大于整个地图大小,函数会锁死
    Vec2 randomPosInMap(const Vec2& pos, float r);
    // enemy(attack_enemy)死亡时调用
    void enemyDie();
    //英雄死亡调用
    void onHeroDie();

public:
    //设置是否暂停状态
    void setIsPause(bool v);

    bool isHeroDead();
    bool isPause();

    void _update();

    void setBarPosition(const Vec2& pos);

    virtual bool init() override;
    virtual void removeFromParent() override;

private:
    void uptateObjects();

    void updateEnemyPointer();

    void updateRedBK();

    void updateBar();

    void updateEnemySerchRadius();

    void updateAddPos();

private:
    Switch switch_isPause;
    Switch switch_heroIsDeid;

    Hero* hero = nullptr;
    //在暂停后即将开始游戏时,可以提前操作控制杆,显示英雄被操作时的虚像
    Sprite* heroPauseShow = nullptr;
    Joystick* joystick = nullptr;
    Sprite* gameMap = nullptr;
    Sprite* bkMap = nullptr;

    //英雄生命值低时会有红色显示
    Sprite* redBk = nullptr;

    //敌人指示器
    Sprite* enemyPointer = nullptr;

    //敌人死亡数
    int enemyDieCount = 0;

    //英雄爆炸图片
    vector<string> heroExplode;

private:  // bar相关
    Sprite* bar_bk = nullptr;

    array<Sprite*, 3> bar_front{nullptr};
    // 0:hp 1:ep 2:progress
    array<Label*, 3> bar_label{nullptr};
    array<LoadingBar*, 3> bar_ui{nullptr};

private:
    Node* particle_parent = nullptr;
    Node* bullet_parent = nullptr;
    Node* enemy_parent = nullptr;
    Node* enemyGroup_parent = nullptr;
    Node* others_parent = nullptr;
    Node* skillEffect_parent = nullptr;

private:
    rand_int* randMap0 = nullptr;
    rand_int* randMap1 = nullptr;

private:
    EnemyWaveManager enemyWaveManager;
    EnemyCreater enemyCreater;
};

#endif