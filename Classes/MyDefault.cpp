#include "header/MyDefault.h"

#include "cocos2d.h"
USING_NS_CC;

_MyDefault MyDefault{};

_Imgs Imgs{};

_MyDefault::_MyDefault()
{
    act.time = 0.2f;
    act.rate = 2.0f;

    font.bold = "fonts/font_bold.otf";
    font.normal = "fonts/font_normal.otf";
    font.medium = "fonts/font_medium.otf";

    option.highFrame = false;
    option.particleQuality = 0;

    hero_bullet01.moveSpeed = 1050;
    hero_bullet01.particleLiveTime_OnMove = 0.25f;
    hero_bullet01.particleCreateSpeed_OnMove = 0.04f;
    hero_bullet01.particleLiveTime_OnExplode = 0.1f;
    hero_bullet01.particleQuantity_OnExplode = 6;
    hero_bullet01.damage = 10;
    hero_bullet01.energy = 2;
    hero_bullet01.range = 900;
    hero_bullet01.attackInterval = 0.15f;

    hero_bullet02.moveSpeed = 1050;
    hero_bullet02.particleLiveTime_OnMove = 0.25f;
    hero_bullet02.particleCreateSpeed_OnMove = 0.04f;
    hero_bullet02.particleLiveTime_OnExplode = 0.1f;
    hero_bullet02.particleQuantity_OnExplode = 6;
    hero_bullet02.damage = 10;
    hero_bullet02.energy = 2;
    hero_bullet02.range = 900;
    hero_bullet02.angleRotateSpeed = 270;
    hero_bullet02.attackInterval = 0.15f;

    enemy01_bullet.moveSpeed = 750;
    enemy01_bullet.particleLiveTime_OnMove = 0.25f;
    enemy01_bullet.particleCreateSpeed_OnMove = 0.05f;
    enemy01_bullet.particleLiveTime_OnExplode = 0.1f;
    enemy01_bullet.particleQuantity_OnExplode = 4;
    enemy01_bullet.damage = 8;

    enemy02_bullet.bulletFrame = "enemy_bullet_02.png";
    enemy02_bullet.physicsInfo = "enemy_bullet_02";
    enemy02_bullet.moveSpeed = 900;
    enemy02_bullet.particleLiveTime_OnMove = 0.25f;
    enemy02_bullet.particleCreateSpeed_OnMove = 0.05f;
    enemy02_bullet.particleLiveTime_OnExplode = 0.1f;
    enemy02_bullet.particleQuantity_OnExplode = 4;
    enemy02_bullet.damage = 2;

    heroSkill_bullet.physicsInfo = "hero_skill_bullet";
    heroSkill_bullet.moveSpeed = 1500;
    heroSkill_bullet.particleLiveTime_OnMove = 0.25f;
    heroSkill_bullet.particleCreateSpeed_OnMove = 0.03f;
    heroSkill_bullet.particleLiveTime_OnExplode = 0.1f;
    heroSkill_bullet.particleQuantity_OnExplode = 8;
    heroSkill_bullet.range = 900;
    heroSkill_bullet.damage = 10;

    enemy01.hitPoint = 130;
    enemy01.particleLiveTime_OnExplode = 0.4f;
    enemy01.particleQuantity_OnExplode = 12;

    enemy02.hitPoint = 140;
    enemy02.particleLiveTime_OnExplode = 0.4f;
    enemy02.particleQuantity_OnExplode = 12;

    enemy02_chlid.physicsInfo = "enemy_02_child";
    enemy02_chlid.particleLiveTime_OnExplode = 0.1f;
    enemy02_chlid.particleQuantity_OnExplode = 3;

    heroExplode.particleLiveTime_OnExplode = 2.5f;
    heroExplode.particleQuantity_OnExplode = 20;

    gameState.isOnGame = false;
    gameState.money = 0;
    gameState.enemyKilled = 0;
    gameState.exp_level = 0;
    gameState.exp_value = 0;
    gameState.exp_nowLevelExpNeed = {0, 100, 200, 400, 800, 1000, 2000, -1};

    myShader.defaultShader =
        GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP;
    myShader.white = "white";
    myShader.light = "light";
    myShader.blur = "blur";
    myShader.outGlow = "outGlow";

    updateInfo.attackSpeed.maxLevel = 6;
    updateInfo.attackSpeed.nowLevel = 0;
    updateInfo.attackSpeed.nowValue = 0;
    updateInfo.attackSpeed.addValue = {0, 6, 7, 8, 9, 10, 10, 0};
    updateInfo.attackSpeed.moneyCost = {0, 50, 100, 160, 400, 600, 800, -1};
    updateInfo.attackSpeed.expCost = {0, 0, 1, 2, 4, 5, 5, 0};

    updateInfo.attackDamage.maxLevel = 5;
    updateInfo.attackDamage.nowLevel = 0;
    updateInfo.attackDamage.nowValue = 0;
    updateInfo.attackDamage.addValue = {0, 5, 6, 7, 8, 9, 0};
    updateInfo.attackDamage.moneyCost = {0, 75, 100, 150, 250, 500, -1};
    updateInfo.attackDamage.expCost = {0, 0, 2, 3, 5, 5, 0};

    updateInfo.defendSubDamage.maxLevel = 4;
    updateInfo.defendSubDamage.nowLevel = 0;
    updateInfo.defendSubDamage.nowValue = 0;
    updateInfo.defendSubDamage.addValue = {0, 4, 5, 5, 6, 0};
    updateInfo.defendSubDamage.moneyCost = {0, 100, 125, 150, 250, -1};
    updateInfo.defendSubDamage.expCost = {0, 1, 2, 3, 5, 0};

    skillShield.particleLiveTime_onStart = 1.5f;
    skillShield.particleLiveTime_onMove = 0.8f;
    skillShield.particleQuantity_onStart = 12;
    skillShield.physicsInfo = "skill_shield";

    enemyWave.attackDamageAdd = 0.1;
    enemyWave.hitPointAdd = 0.1;
    enemyWave.baseEnemyNumbers = 5;
}

_Imgs::_Imgs()
{
    share.t_bkGround = "bkground_0.png";
    share.t_blackBkGround = "blackground.png";
    share.p_coin = "coin.png";
    share.p_exp = "exp.png";

    loadingPage.t_plane = "loading_plane.png";
    loadingPage.t_loadingBK = "loading_bkground.png";
    homePage.p_title = "home_title.png";
    homePage.t_effect = "home_decorate0.png";
    homePage.vp_startButton = {"home_start_button0.png",
                               "home_start_button1.png"};
    homePage.vp_optionButton = {"home_option_button0.png",
                                "home_option_button1.png"};

    optionPage.t_infoBK = "option_info_bkground.png";
    optionPage.p_title = "option_option.png";
    optionPage.p_ui0_BK = "ui_optionBar_show_bk.png";
    optionPage.vp_ui0_ele0 = {"ui_optionBar_switch_bk.png",
                              "ui_optionBar_switch_off.png",
                              "ui_optionBar_switch_on.png"};
    optionPage.vp_ui0_ele1 = {
        "ui_optionBar_switch3_bk.png", "ui_optionBar_switch3_low.png",
        "ui_optionBar_switch3_mid.png", "ui_optionBar_switch3_high.png"};
    optionPage.vp_returnButton = {"return_button0.png", "return_button1.png"};

    gameLobby.t_cutLine = "game_lobby_cutLine.png";
    gameLobby.t_updateAttackBK = "game_lobby_attack_bk.png";
    gameLobby.t_updateDefendBk = "game_lobby_defend_bk.png";
    gameLobby.p_effect = "game_lobby_start_effect.png";
    gameLobby.p_attack_update_bk = "game_lobby_update_bk.png";
    gameLobby.p_moneyShowBK = "game_lobby_money_bk.png";
    gameLobby.vp_returnButton = {"game_lobby_return_button0.png",
                                 "game_lobby_return_button1.png"};
    gameLobby.vp_startButton = {"game_lobby_start_button0.png",
                                "game_lobby_start_button1.png"};
    gameLobby.vp_updateButton = {"game_lobby_update_button0.png",
                                 "game_lobby_update_button1.png"};
    gameLobby.vp_updateBar = {"game_lobby_update_bar_bk.png",
                              "game_lobby_update_bar_cover.png",
                              "game_lobby_update_bar.png"};

    gameOver.t_overBK = "game_over_bk.png";
    gameOver.vp_endButton = {"game_over_button0.png", "game_over_button1.png"};

    gamePage0.t_gameBK = "game_bkground.png";
    gamePage0.t_gameMap = "game_map.png";
    gamePage0.t_gameRedBK = "game_red_bk.png";
    gamePage0.p_enemyPointer = "enemy_pointer.png";
    gamePage0.p_joystick_arrow = "hero_skill_joystick_arrow.png";
    gamePage0.p_joystick_attack_disable = "joystick_attack_disable.png";
    gamePage0.vp_enemyHitPointBar = {"enemy_hitPointBar.png",
                                     "enemy_hitPointBar_bk.png"};
    gamePage0.vp_bar = {"bar_bk.png", "bar_front.png", "bar_hp.png",
                        "bar_ep.png", "bar_progress.png"};

    gamePage0.joystick.vp_move = {
        "joystick_large.png",
        "joystick_small.png",
    };

    gamePage0.joystick.vp_attack = {"joystick_attack_bk.png",
                                    "joystick_attack.png"};
    gamePage0.joystick.vp_skill = {
        "hero_skill_joysitck_button.png", "hero_skill_joysitck_bk.png",
        "skill_shield_button0.png", "skill_shield_button1.png",
        "skill_shield_button2.png"};

    gamePage1.t_pauseBK = "game_info_bkground.png";
    gamePage1.vp_pauseButton = {"game_pause_button0.png",
                                "game_pause_button1.png"};
    gamePage1.vp_startButton = {"game_start_button0.png",
                                "game_start_button1.png"};
    gamePage1.vp_mainPageButton = {"game_menu_mainPage_button0.png",
                                   "game_menu_mainPage_button1.png"};
    gamePage1.vp_mainPageAcceptButton = {
        "game_menu_mainPage_accept_button0.png",
        "game_menu_mainPage_accept_button1.png"};
    gamePage1.vp_mainPageCancelButton = {
        "game_menu_mainPage_cancel_button0.png",
        "game_menu_mainPage_cancel_button1.png"};

    ////////////////
    // hero
    onGame.hero.p_hero = "hero.png";
    onGame.hero.p_cannon = "hero_cannon.png";
    onGame.hero.vp_explode = {"enemy_kill_explode0.png",
                              "enemy_kill_explode1.png"};

    ////////////////
    // enemy0
    onGame.enemy0.p_enemy = "enemy.png";
    onGame.enemy0.vp_explode = {"enemy_kill_explode0.png",
                                "enemy_kill_explode1.png"};

    ////////////////
    // enemy1
    onGame.enemy1.p_enemy = "enemy_02.png";
    onGame.enemy1.vp_explode = {"enemy_kill_explode0.png",
                                "enemy_kill_explode1.png"};

    ////////////////
    // enemy1_child
    onGame.enemy1Child.p_enemy = "enemy_02_child.png";
    onGame.enemy1Child.vp_explode = {"enemy_kill_explode0.png",
                                     "enemy_kill_explode1.png"};

    ////////////////
    // exp
    onGame.exp.p_hp = "enemy_exp_hp.png";
    onGame.exp.p_ep = "enemy_exp_ep.png";

    ////////////////
    // heroBullet0
    onGame.heroBullet0.p_bullet = "hero_bullet.png";
    onGame.heroBullet0.vp_explode = {"hero_explode_particle0.png",
                                     "hero_explode_particle1.png"};
    onGame.heroBullet0.vp_move = {"hero_move_particle0.png",
                                  "hero_move_particle1.png",
                                  "hero_move_particle2.png"};

    ////////////////
    // enemyBullet0
    onGame.enemyBullet0.p_bullet = "enemy_bullet.png";
    onGame.enemyBullet0.vp_explode = {"enemy_bullet0_explode_particle0.png",
                                      "enemy_bullet0_explode_particle1.png"};
    onGame.enemyBullet0.vp_move = {"enemy_bullet0_move_particle0.png",
                                   "enemy_bullet0_move_particle1.png",
                                   "enemy_bullet0_move_particle2.png"};

    ////////////////
    // enemyBullet1
    onGame.enemyBullet1.p_bullet = "enemy_bullet_02.png";
    onGame.enemyBullet1.vp_explode = {"enemy_bullet1_explode_particle0.png",
                                      "enemy_bullet1_explode_particle1.png"};
    onGame.enemyBullet1.vp_move = {"enemy_bullet1_move_particle0.png",
                                   "enemy_bullet1_move_particle1.png",
                                   "enemy_bullet1_move_particle2.png"};

    onGame.enemyGroup.p_group = "enemy_group.png";
    onGame.enemyGroup.p_lineDot = "enemy_group_lineDot.png";
    onGame.enemyGroup.p_circle = "enemy_group_circle.png";

    ////////////////
    // skill_shield
    onGame.skillShield.t_background = "game_skill_shield_bk.png";
    onGame.skillShield.t_shake = "game_skill_shield_shake.png";
    onGame.skillShield.p_shield = "skill_shield.png";
    onGame.skillShield.p_bar = "skill_shield_bar.png";
    onGame.skillShield.p_bar_bk = "skill_shield_bar_bk.png";
    onGame.skillShield.vp_effect = {"skill_shield_effect0.png",
                                    "skill_shield_effect1.png",
                                    "skill_shield_effect2.png"};

    onGame.heroSkillBullet.p_bullet = "hero_skill_bullet.png";
    onGame.heroSkillBullet.vp_explode = {"enemy_bullet0_explode_particle0.png",
                                         "enemy_bullet0_explode_particle1.png"};
    onGame.heroSkillBullet.vp_move = {"enemy_bullet0_move_particle0.png",
                                      "enemy_bullet0_move_particle1.png",
                                      "enemy_bullet0_move_particle2.png"};
}
