#include "header/GameOverPage.h"
USING_NS_CC;
#include "ui/UIRichText.h"
using namespace cocos2d::ui;

#include <cstdlib>

#include "header/GameLobby.h"
#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/XMLManager.h"

enum _zORDER { z_bkground, z_overBK, z_Button, z_blackbkground };

cocos2d::Scene* GameOver::createScene() { return GameOver::create(); }

bool GameOver::init()
{
    if (!Scene::init()) {
        return false;
    }
    auto l0 = GameOverLayer0::create();
    this->addChild(l0, 0);

    return true;
}

bool GameOverLayer0::init()
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
    [&]() {
        auto f = FadeOut::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([&]() { this->enableAllButtons(); });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    }();
    this->addChild(blackBkground, _zORDER::z_blackbkground);

    bkground =
        Sprite::createWithTexture(texture->getTextureForKey("bkground_0.png"));
    bkground->setPosition(visibleSize / 2);
    this->addChild(bkground, z_bkground);

    auto overBK = Sprite::create(Imgs.gameOver.t_overBK);
    overBK->setPosition(960, visibleSize.height - 432);
    this->addChild(overBK, z_overBK);

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, z_Button);

    //创建结束加分按钮
    {
        auto s0 =
            Sprite::createWithSpriteFrameName(Imgs.gameOver.vp_endButton[0]);
        auto s1 =
            Sprite::createWithSpriteFrameName(Imgs.gameOver.vp_endButton[1]);

        auto end = MenuItemSprite::create(s0, s1, [&](Ref*) {
            this->disableAllButtons();

            auto f = FadeIn::create(MyDefault.act.time);
            auto act = EaseIn::create(f, MyDefault.act.rate);
            auto func = CallFunc::create([]() {
                auto sence = GameLobby::createScene();
                Director::getInstance()->replaceScene(sence);
            });
            auto s = Sequence::create(act, func, nullptr);
            blackBkground->runAction(s);
        });
        end->setPosition(960, visibleSize.height - 960);
        end->setEnabled(false);
        menu->addChild(end);
        this->addButton(end);
    }

    {
        bool isFullLevel = false;
        int coinAdd =
            MyMath::floatToInt(MyDefault.gameState.enemyKilled * 1.5f);
        int expAdd = MyMath::floatToInt(MyDefault.gameState.enemyKilled * 2);

        //经验满级时,一些经验转化为金币
        int exp_coin_Add = 0;

        {
            while (1) {
                if (MyDefault.gameState
                        .exp_nowLevelExpNeed[MyDefault.gameState.exp_level +
                                             1] != -1 /*说明还没满级*/) {
                    //当前加的经验超过需要的,要升级了
                    if (MyDefault.gameState.exp_value + expAdd >=
                        MyDefault.gameState
                            .exp_nowLevelExpNeed[MyDefault.gameState.exp_level +
                                                 1]) {
                        MyDefault.gameState.exp_value += expAdd;
                        MyDefault.gameState.exp_value -=
                            MyDefault.gameState.exp_nowLevelExpNeed
                                [MyDefault.gameState.exp_level + 1];

                        MyDefault.gameState.exp_level += 1;
                    } else {
                        MyDefault.gameState.exp_value += expAdd;
                        break;
                    }
                } else {
                    isFullLevel = true;
                    exp_coin_Add = MyMath::floatToInt(expAdd * 0.25f);
                    break;
                }
            }
        }

        MyDefault.gameState.money += (coinAdd + exp_coin_Add);

        auto coinAdd_richText = RichText::create();
        coinAdd_richText->setAnchorPoint(Vec2(0, 1));
        coinAdd_richText->setPosition(Vec2(170, visibleSize.height - 450));
        this->addChild(coinAdd_richText, z_Button);

        char str[64] = "";
        std::snprintf(str, sizeof(str), "+%d", coinAdd + exp_coin_Add);
        auto coinAdd_label = RichElementText::create(
            0, Color3B(219, 219, 219), 255, str, MyDefault.font.medium, 65);
        coinAdd_richText->pushBackElement(coinAdd_label);

        if (isFullLevel) {
            char str[64] = "";
            std::snprintf(str, sizeof(str), "(+%d)", exp_coin_Add);
            auto coinAdd_label = RichElementText::create(
                0, Color3B(115, 230, 30), 255, str, MyDefault.font.medium, 60);
            coinAdd_richText->pushBackElement(coinAdd_label);
        }

        if (!isFullLevel) {
            auto expAdd_label = Label::createWithTTF(
                "", MyDefault.font.medium, 65, Size(250, 0),
                TextHAlignment::LEFT, TextVAlignment::CENTER);
            expAdd_label->setAnchorPoint(Vec2(0, 1));
            expAdd_label->setPosition(170, visibleSize.height - 614);
            expAdd_label->setTextColor(Color4B(219, 219, 219, 255));
            this->addChild(expAdd_label, z_Button);

            char str0[64] = "";
            std::snprintf(str0, sizeof(str0), "+%d", expAdd);
            expAdd_label->setString(str0);
        } else {
            auto expAdd_label = Label::createWithTTF(
                "", MyDefault.font.medium, 40, Size(450, 0),
                TextHAlignment::LEFT, TextVAlignment::CENTER);
            expAdd_label->setAnchorPoint(Vec2(0, 1));
            expAdd_label->setPosition(170, visibleSize.height - 614);
            expAdd_label->setTextColor(Color4B(219, 219, 219, 255));
            this->addChild(expAdd_label, z_Button);

            char str0[64] = "";
            std::snprintf(str0, sizeof(str0), "%s",
                          XMLManager::getString("game_over_full_exp").c_str());
            expAdd_label->setString(str0);
        }

        auto runInThread = [=]() {
            //写入文件
            auto UD = UserDefault::getInstance();
            UD->setIntegerForKey("money", MyDefault.gameState.money);
            UD->setIntegerForKey("exp_level", MyDefault.gameState.exp_level);
            UD->setIntegerForKey("exp_value", MyDefault.gameState.exp_value);
            UD->flush();
        };
        //创建写入文件的线程
        thread th(runInThread);
        th.detach();
    }

    return true;
}
