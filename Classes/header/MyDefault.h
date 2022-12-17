#ifndef _MY_DEFAULT_H_
#define _MY_DEFAULT_H_

#include <string>
#include <thread>
#include <vector>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

inline string shaderProgramFilePath(const string& file)
{
    return "Shader/" + file;
}

struct _MyDefault {
    //完成所有变量的初始化
    _MyDefault();

    struct act_info {
        float time;
        float rate;
    } act;

    struct font_info {
        string bold;
        string normal;
        string medium;
    } font;

    struct option_info {
        //高帧率模式
        bool highFrame;
        //粒子质量, 0无 1低 2高
        int particleQuality;
    } option;

    struct hero_bullet_info01 {
        float moveSpeed;
        float particleLiveTime_OnMove;

        //会被粒子质量改动
        float particleCreateSpeed_OnMove;

        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;

        float damage;
        int energy;
        int range;
        float attackInterval;
    } hero_bullet01;

    struct hero_bullet_info02 {
        float moveSpeed;
        float particleLiveTime_OnMove;

        //会被粒子质量改动
        float particleCreateSpeed_OnMove;

        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;

        float damage;
        int energy;
        int range;
        float angleRotateSpeed;
        float attackInterval;
    } hero_bullet02;

    struct enemy01_bullet_info {
        float moveSpeed;
        float particleLiveTime_OnMove;
        //会被粒子质量改动
        float particleCreateSpeed_OnMove;

        float particleLiveTime_OnExplode;
        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;

        int damage;
    } enemy01_bullet;

    struct enemy02_bullet_info {
        string bulletFrame;
        string physicsInfo;

        float moveSpeed;
        float particleLiveTime_OnMove;
        //会被粒子质量改动
        float particleCreateSpeed_OnMove;

        float particleLiveTime_OnExplode;
        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;

        int damage;
    } enemy02_bullet;

    struct _heroSkillBullet {
        string physicsInfo;
        float moveSpeed;
        float particleLiveTime_OnMove;

        //会被粒子质量改动
        float particleCreateSpeed_OnMove;

        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;

        float damage;
        int range;
    } heroSkill_bullet;

    struct enemy01_info {
        int hitPoint = 130;

        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;
    } enemy01;

    struct enemy02_info {
        int hitPoint = 130;
        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;
    } enemy02;

    struct enemy02_chlid_info {
        string physicsInfo;

        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;
    } enemy02_chlid;

    //英雄死亡爆炸
    struct hero_explode_info {
        float particleLiveTime_OnExplode;

        //会被粒子质量改动
        unsigned int particleQuantity_OnExplode;
    } heroExplode;

    struct game_state {
        bool isOnGame;
        int money;
        int exp_level;
        int exp_value;
        vector<int> exp_nowLevelExpNeed;

        //敌人击杀数
        int enemyKilled;
    } gameState;

    struct my_shader {
        string defaultShader;
        string white;
        string light;
        string blur;
        string outGlow;
    } myShader;

    struct update_info {
        struct _base_update {
            int maxLevel;
            int nowLevel;
            float nowValue;
            vector<float> addValue;
            vector<int> moneyCost;
            vector<int> expCost;
        } attackSpeed /*攻速*/, attackDamage /*伤害*/,
            defendSubDamage /*几率不受伤害*/;

    } updateInfo;

    struct _skill_shield {
        float particleLiveTime_onStart;
        float particleLiveTime_onMove;
        string physicsInfo;
        //会被粒子质量改动
        unsigned int particleQuantity_onStart;

        float shakeDistance;
        float damage;
        float hitPoint;
    } skillShield;

    //敌人波数
    struct enemy_wave {
        float hitPointAdd;
        float attackDamageAdd;

        //基础敌人数量
        int baseEnemyNumbers;
    } enemyWave;

    const Size designResolutionSize{1920, 1080};
};

extern _MyDefault MyDefault;

//图片命名规则
// t_xxx  texture
// p_xxx spriteFrame
// vp_xxx vector spriteFrame

struct _Imgs {
    _Imgs();

    struct _share {
        string t_bkGround;
        string t_blackBkGround;
        string p_coin;
        string p_exp;
    } share;

    struct _loadingPage {
        string t_plane;
        string t_loadingBK;
    } loadingPage;

    struct _homePage {
        string p_title;
        string t_effect;
        vector<string> vp_startButton;
        vector<string> vp_optionButton;
    } homePage;

    struct _optionPage {
        string t_infoBK;
        string p_title;
        string p_ui0_BK;
        vector<string> vp_ui0_ele0;
        vector<string> vp_ui0_ele1;
        vector<string> vp_returnButton;
    } optionPage;

    struct _gameLobby {
        string t_cutLine;
        string t_updateAttackBK;
        string t_updateDefendBk;
        string p_effect;
        string p_moneyShowBK;
        string p_attack_update_bk;
        vector<string> vp_returnButton;
        vector<string> vp_startButton;
        vector<string> vp_updateButton;
        vector<string> vp_updateBar;
    } gameLobby;

    struct _gameOver {
        string t_overBK;
        vector<string> vp_endButton;
    } gameOver;

    struct _gamePage0 {  //游戏基本元素(无精灵)
        string t_gameBK;
        string t_gameMap;
        string t_gameRedBK;
        string p_enemyPointer;
        string p_joystick_arrow;
        string p_joystick_attack_disable;
        vector<string> vp_enemyHitPointBar;
        vector<string> vp_bar;

        struct {
            vector<string> vp_move;
            vector<string> vp_attack;
            vector<string> vp_skill;
        } joystick;
    } gamePage0;

    struct _gamePage1 {
        string t_pauseBK;
        vector<string> vp_pauseButton;
        vector<string> vp_startButton;
        vector<string> vp_mainPageButton;
        vector<string> vp_mainPageAcceptButton;
        vector<string> vp_mainPageCancelButton;

    } gamePage1;

    struct _onGame {  //游戏精灵

        struct _hero {
            string p_hero;
            string p_cannon;
            vector<string> vp_explode;
        } hero;

        struct _heroBullet0 {
            string p_bullet;
            vector<string> vp_explode;
            vector<string> vp_move;
        } heroBullet0;

        struct _heroSkillBullet {
            string p_bullet;
            vector<string> vp_explode;
            vector<string> vp_move;
        } heroSkillBullet;

        struct _enemyBullet0 {
            string p_bullet;
            vector<string> vp_explode;
            vector<string> vp_move;
        } enemyBullet0;

        struct _enemyBullet1 {
            string p_bullet;
            vector<string> vp_explode;
            vector<string> vp_move;
        } enemyBullet1;

        struct _enemy0 {
            string p_enemy;
            vector<string> vp_explode;
        } enemy0;

        struct _enemy1 {
            string p_enemy;
            vector<string> vp_explode;
        } enemy1;

        struct _enemy1Child {
            string p_enemy;
            vector<string> vp_explode;
        } enemy1Child;

        struct _exp {
            string p_hp;
            string p_ep;
        } exp;

        struct _enemyGroup {
            string p_group;
            string p_lineDot;
            string p_circle;
        } enemyGroup;

        struct _skillShield {
            string t_background;
            string t_shake;
            string p_shield;
            string p_bar;
            string p_bar_bk;
            vector<string> vp_effect;
        } skillShield;

    } onGame;
};

extern _Imgs Imgs;

#endif