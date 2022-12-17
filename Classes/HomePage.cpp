#include "header/HomePage.h"

#include "header/GameLobby.h"
#include "header/MyDefault.h"
#include "header/OptionPage.h"

USING_NS_CC;

enum _zORDER { z_bkground, z_title, z_decorate0, z_Button, z_blackbkground };

cocos2d::Scene* HomePage::createScene() { return HomePage::create(); }

bool HomePage::init()
{
    if (!Scene::create()) {
        return false;
    }

    auto layer0 = HomePageLayer0::create();
    this->addChild(layer0, 0);

    return true;
}

bool HomePageLayer0::init()
{
    if (!Layer::create()) {
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

    auto bkground = Sprite::createWithTexture(
        texture->getTextureForKey(Imgs.share.t_bkGround));
    bkground->setPosition(visibleSize / 2);
    this->addChild(bkground, z_bkground);

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, z_Button);

    auto title = Sprite::createWithSpriteFrameName(Imgs.homePage.p_title);
    title->setPosition(visibleSize.width / 2,
                       visibleSize.height - title->getContentSize().height / 2 -
                           title->getContentSize().height / 4);
    this->addChild(title, z_title);

    auto start0 =
        Sprite::createWithSpriteFrameName(Imgs.homePage.vp_startButton[0]);
    auto start1 =
        Sprite::createWithSpriteFrameName(Imgs.homePage.vp_startButton[1]);
    auto start_button = MenuItemSprite::create(start0, start1, [this](Ref*) {
        this->disableAllButtons();

        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([&]() {
            auto sence = GameLobby::createScene();
            Director::getInstance()->replaceScene(sence);
        });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    });
    start_button->setPosition(visibleSize.width / 2,
                              title->getPosition().y -
                                  title->getContentSize().height / 2 -
                                  start_button->getContentSize().height / 1.2f);
    this->addButton(start_button);
    menu->addChild(start_button);

    auto option0 =
        Sprite::createWithSpriteFrameName(Imgs.homePage.vp_optionButton[0]);
    auto option1 =
        Sprite::createWithSpriteFrameName(Imgs.homePage.vp_optionButton[1]);
    auto option_button = MenuItemSprite::create(option0, option1, [this](Ref*) {
        this->disableAllButtons();

        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([]() {
            auto sence = OptionPage::createScene();
            Director::getInstance()->replaceScene(sence);
        });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    });
    option_button->setPosition(
        visibleSize.width / 2,
        start_button->getPosition().y - start_button->getContentSize().height);
    this->addButton(option_button);
    menu->addChild(option_button);

    auto decorate0 = Sprite::createWithTexture(
        texture->getTextureForKey(Imgs.homePage.t_effect));
    decorate0->setPosition(
        visibleSize.width / 2,
        start_button->getPosition().y -
            (start_button->getPosition().y - option_button->getPosition().y) /
                2);
    this->addChild(decorate0, z_decorate0);

    this->disableAllButtons();

    //////////////////////////////////

    return true;
}
