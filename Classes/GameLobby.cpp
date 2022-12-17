#include "header/GameLobby.h"

#include "header/GamePage.h"
#include "header/HomePage.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/Random.h"
#include "header/UI_Store.h"
#include "header/XMLManager.h"
#include "ui/UIRichText.h"
using namespace cocos2d::ui;

enum _zORDER {
    z_bkground,
    z_updateBK,
    z_updateSmallBK,
    z_text,
    z_Button,
    z_start_effect,
    z_cutLine,
    z_effect,
    z_blackbkground
};

Scene* GameLobby::createScene() { return GameLobby::create(); }

bool GameLobby::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto l0 = GameLobbyLayer0::create();
    this->addChild(l0, 0);

    return true;
}

bool GameLobbyLayer0::init()
{
    if (!Layer::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    auto const texture = Director::getInstance()->getTextureCache();
    //////////////////////////////////

    blackBkground = Sprite::createWithTexture(
        texture->getTextureForKey(Imgs.share.t_blackBkGround));
    blackBkground->setPosition(visibleSize / 2);
    {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([&]() { this->enableAllButtons(); });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    }
    this->addChild(blackBkground, _zORDER::z_blackbkground);

    bkground =
        Sprite::createWithTexture(texture->getTextureForKey("bkground_0.png"));
    bkground->setPosition(visibleSize / 2);
    this->addChild(bkground, z_bkground);

    auto cutLine = Sprite::createWithTexture(
        texture->getTextureForKey("game_lobby_cutLine.png"));
    cutLine->setPosition(960, visibleSize.height - 520);
    this->addChild(cutLine, z_cutLine);

    auto start_button_effect =
        Sprite::createWithSpriteFrameName("game_lobby_start_effect.png");
    start_button_effect->setPosition(960, visibleSize.height - 992);
    this->addChild(start_button_effect, z_start_effect);

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, z_Button);

    {
        auto b0 =
            Sprite::createWithSpriteFrameName("game_lobby_return_button0.png");
        auto b1 =
            Sprite::createWithSpriteFrameName("game_lobby_return_button1.png");
        MenuItemSprite* return_button =
            MenuItemSprite::create(b0, b1, [&](Ref*) {
                this->disableAllButtons();
                auto f = FadeIn::create(MyDefault.act.time);
                auto act = EaseIn::create(f, MyDefault.act.rate);
                auto func = CallFunc::create([&]() {
                    auto sence = HomePage::createScene();
                    Director::getInstance()->replaceScene(sence);
                });
                auto s = Sequence::create(act, func, nullptr);
                blackBkground->runAction(s);
            });
        return_button->setPosition(195, visibleSize.height - 995);
        menu->addChild(return_button);
        this->addButton(return_button);
    }

    {
        auto b0 =
            Sprite::createWithSpriteFrameName("game_lobby_start_button0.png");
        auto b1 =
            Sprite::createWithSpriteFrameName("game_lobby_start_button1.png");
        auto start_button = MenuItemSprite::create(b0, b1, [&](Ref*) {
            this->disableAllButtons();
            auto f = FadeIn::create(MyDefault.act.time);
            auto act = EaseIn::create(f, MyDefault.act.rate);
            auto func = CallFunc::create([&]() {
                auto sence = GamePage::createScene();
                Director::getInstance()->replaceScene(sence);
            });
            auto s = Sequence::create(act, func, nullptr);
            blackBkground->runAction(s);
        });
        start_button->setPosition(960, visibleSize.height - 992);
        menu->addChild(start_button);
        this->addButton(start_button);
    }
    this->disableAllButtons();

    //升级,攻击栏的背景
    auto updateAttackBk = Sprite::create(Imgs.gameLobby.t_updateAttackBK);
    updateAttackBk->setPosition(410, visibleSize.height - 522);
    this->addChild(updateAttackBk, z_updateBK);

    //升级,攻击栏的背景
    auto updateDefendBk = Sprite::create(Imgs.gameLobby.t_updateDefendBk);
    updateDefendBk->setPosition(953, visibleSize.height - 522);
    this->addChild(updateDefendBk, z_updateBK);

    //第一个升级UI
    {
        auto update_AttackSpeed = UI_Store_updateBar::create(
            Imgs.gameLobby.p_attack_update_bk, MyDefault.updateInfo.attackSpeed,
            "update_attackSpeed_level");
        update_AttackSpeed->createInfo(
            40, XMLManager::getString("lobby_update_attack01_str0"),
            XMLManager::getString("lobby_update_attack01_str1"));
        update_AttackSpeed->createButton(
            Imgs.gameLobby.vp_updateBar[0], Imgs.gameLobby.vp_updateBar[1],
            Imgs.gameLobby.vp_updateBar[2], Imgs.gameLobby.vp_updateButton[0],
            Imgs.gameLobby.vp_updateButton[1]);
        update_AttackSpeed->setBKPosition(Vec2(410, visibleSize.height - 470));
        update_AttackSpeed->setButtonBarPosition(
            Vec2(301, visibleSize.height - 528));
        update_AttackSpeed->setInfoBarPosition(
            Vec2(187, visibleSize.height - 385));
        update_AttackSpeed->setUpdateFinishCallBack([&, update_AttackSpeed]() {
            char str[32] = "";
            std::snprintf(str, sizeof(str), "%d", MyDefault.gameState.money);
            moneyShow->setString(str);
            //创建特效
            this->particleOnUpdate(update_AttackSpeed->getButtonPos());
        });
        update_AttackSpeed->refreshInfo();

        MyDefault.updateInfo.attackSpeed.nowValue =
            update_AttackSpeed->getNowValue();
        this->addChild(update_AttackSpeed, z_text);
    }

    //第二个升级UI
    {
        auto update_AttackDamage = UI_Store_updateBar::create(
            Imgs.gameLobby.p_attack_update_bk,
            MyDefault.updateInfo.attackDamage, "update_attackDamage_level");
        update_AttackDamage->createInfo(
            40, XMLManager::getString("lobby_update_attack02_str0"),
            XMLManager::getString("lobby_update_attack02_str1"));
        update_AttackDamage->createButton(
            Imgs.gameLobby.vp_updateBar[0], Imgs.gameLobby.vp_updateBar[1],
            Imgs.gameLobby.vp_updateBar[2], Imgs.gameLobby.vp_updateButton[0],
            Imgs.gameLobby.vp_updateButton[1]);
        update_AttackDamage->setBKPosition(Vec2(410, visibleSize.height - 703));
        update_AttackDamage->setButtonBarPosition(
            Vec2(301, visibleSize.height - 761));
        update_AttackDamage->setInfoBarPosition(
            Vec2(187, visibleSize.height - 617));
        update_AttackDamage->setUpdateFinishCallBack([&,
                                                      update_AttackDamage]() {
            char str[32] = "";
            std::snprintf(str, sizeof(str), "%d", MyDefault.gameState.money);
            moneyShow->setString(str);
            //创建特效
            this->particleOnUpdate(update_AttackDamage->getButtonPos());
        });
        update_AttackDamage->refreshInfo();
        MyDefault.updateInfo.attackDamage.nowValue =
            update_AttackDamage->getNowValue();
        this->addChild(update_AttackDamage, z_text);
    }

    //第三个升级UI
    {
        auto update_defendSubDamage =
            UI_Store_updateBar::create(Imgs.gameLobby.p_attack_update_bk,
                                       MyDefault.updateInfo.defendSubDamage,
                                       "update_defendSubDamage_level");
        update_defendSubDamage->createInfo(
            40, XMLManager::getString("lobby_update_attack03_str0"),
            XMLManager::getString("lobby_update_attack03_str1"));
        update_defendSubDamage->createButton(
            Imgs.gameLobby.vp_updateBar[0], Imgs.gameLobby.vp_updateBar[1],
            Imgs.gameLobby.vp_updateBar[2], Imgs.gameLobby.vp_updateButton[0],
            Imgs.gameLobby.vp_updateButton[1]);
        update_defendSubDamage->setBKPosition(
            Vec2(953, visibleSize.height - 470));
        update_defendSubDamage->setButtonBarPosition(
            Vec2(843, visibleSize.height - 528));
        update_defendSubDamage->setInfoBarPosition(
            Vec2(729, visibleSize.height - 385));
        update_defendSubDamage->setUpdateFinishCallBack(
            [&, update_defendSubDamage]() {
                char str[32] = "";
                std::snprintf(str, sizeof(str), "%d",
                              MyDefault.gameState.money);
                moneyShow->setString(str);
                //创建特效
                this->particleOnUpdate(update_defendSubDamage->getButtonPos());
            });
        update_defendSubDamage->refreshInfo();
        MyDefault.updateInfo.defendSubDamage.nowValue =
            update_defendSubDamage->getNowValue();
        this->addChild(update_defendSubDamage, z_text);
    }

    //金币显示背景
    auto moneyBK =
        Sprite::createWithSpriteFrameName(Imgs.gameLobby.p_moneyShowBK);
    moneyBK->setPosition(232, visibleSize.height - 64);
    this->addChild(moneyBK, z_updateBK);

    //经验显示背景
    auto expBK =
        Sprite::createWithSpriteFrameName(Imgs.gameLobby.p_moneyShowBK);
    expBK->setPosition(654, visibleSize.height - 64);
    this->addChild(expBK, z_updateBK);

    //金币图标
    auto coin = Sprite::createWithSpriteFrameName(Imgs.share.p_coin);
    coin->setPosition(93, visibleSize.height - 64);
    this->addChild(coin, z_updateBK);

    //经验图标
    auto exp = Sprite::createWithSpriteFrameName(Imgs.share.p_exp);
    exp->setPosition(519, visibleSize.height - 64);
    this->addChild(exp, z_updateBK);

    //金币数量显示
    moneyShow =
        Label::createWithTTF("", MyDefault.font.medium, 38, Size(250, 0),
                             TextHAlignment::LEFT, TextVAlignment::CENTER);
    moneyShow->setTextColor(Color4B(220, 220, 220, 255));
    char str0[32] = "";
    std::snprintf(str0, sizeof(str0), "%d", MyDefault.gameState.money);
    moneyShow->setString(str0);

    moneyShow->setAnchorPoint(Vec2(0, 0.5));
    moneyShow->setPosition(145, visibleSize.height - 64);
    this->addChild(moneyShow, z_updateBK);

    //经验数量显示
    auto expShow =
        Label::createWithTTF("", MyDefault.font.medium, 34, Size(250, 0),
                             TextHAlignment::LEFT, TextVAlignment::CENTER);
    expShow->setTextColor(Color4B(220, 220, 220, 255));
    char str1[32] = "";
    auto expNeed = MyDefault.gameState
                       .exp_nowLevelExpNeed[MyDefault.gameState.exp_level + 1];
    if (expNeed != -1) {  //如果没满级
        std::snprintf(str1, sizeof(str1), "%d%s(%d/%d)",
                      MyDefault.gameState.exp_level + 1,
                      XMLManager::getString("lobby_exp_str0").c_str(),
                      MyDefault.gameState.exp_value, expNeed);
    } else {  //满级了
        std::snprintf(str1, sizeof(str1), "%d%s-%s",
                      MyDefault.gameState.exp_level + 1,
                      XMLManager::getString("lobby_exp_str0").c_str(),
                      XMLManager::getString("lobby_full_exp").c_str());
    }
    expShow->setString(str1);
    expShow->setAnchorPoint(Vec2(0, 0.5));
    expShow->setPosition(567, visibleSize.height - 64);
    this->addChild(expShow, z_updateBK);

    //////////////////////////////////////////////

    return true;
}

void GameLobbyLayer0::particleOnUpdate(const Vec2& pos)
{
    const auto create0 = [&](float dt) {
        auto sp =
            Sprite::createWithSpriteFrameName("game_lobby_update_effect.png");
        sp->setPosition(pos);
        sp->setScale(0);
        this->addChild(sp, z_effect);
        auto a0 = ScaleTo::create(1.0f, 1.0f);
        auto e0 = EaseOut::create(a0, 1.5f);
        auto a1 = FadeOut::create(1.0f);
        auto e1 = EaseOut::create(a1, 1.5f);
        auto a2 =
            CallFuncN::create([](Node* node) { node->removeFromParent(); });
        auto s0 = Spawn::create({e0, e1});
        auto d0 = DelayTime::create(dt);
        auto s = Sequence::create({d0, s0, a2});
        sp->runAction(s);
    };

    //粒子特效
    const auto create1 = [&](const string& frame) {
        auto sp = Sprite::createWithSpriteFrameName(frame);
        sp->setPosition(pos);
        // sp->setScale(0);
        this->addChild(sp, z_effect);

        rand_float r0(100, 150);
        rand_int r1(0, 359);
        const float dis = r0();
        const float angle = r1();
        Vec2 p1 = Vec2(dis * DEG::cos(angle), dis * DEG::sin(angle));

        auto a0 = MoveBy::create(1.0f, p1);
        auto e0 = EaseOut::create(a0, 2);
        auto a1 = ScaleTo::create(1.0f, 0);
        auto a2 =
            CallFuncN::create([](Node* node) { node->removeFromParent(); });

        auto a4 = RotateBy::create(1.0f, 360 * 4);
        auto a3 = Spawn::create({e0, a1, a4});
        auto s = Sequence::create({a3, a2});
        sp->runAction(s);
    };

    for (int x = 0; x < 2; ++x) {
        create0(0.05f + x * 0.1f);
    }

    for (int x = 0; x < 5; ++x) {
        create1("game_lobby_update_particle0.png");
        create1("game_lobby_update_particle1.png");
    }
}
