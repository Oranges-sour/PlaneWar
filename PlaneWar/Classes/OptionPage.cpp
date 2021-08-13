#include "header/OptionPage.h"

#include "header/GamePage.h"
#include "header/HomePage.h"
#include "header/MyDefault.h"
#include "header/XMLManager.h"

USING_NS_CC;

enum _zORDER { z_bkground, z_infoBkground, z_info, z_Button, z_blackbkground };

cocos2d::Scene* OptionPage::createScene() { return OptionPage::create(); }

bool OptionPage::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto layer = OptionPageLayer0::create();
    this->addChild(layer, 0);

    return true;
}

bool OptionPageLayer0::init()
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
        auto func = CallFunc::create([this]() { this->enableAllButtons(); });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    }();
    this->addChild(blackBkground, _zORDER::z_blackbkground);

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, z_Button);

    auto infoBkground = Sprite::createWithTexture(
        texture->getTextureForKey("option_info_bkground.png"));
    infoBkground->setPosition(visibleSize.width / 2,
                              visibleSize.height -
                                  infoBkground->getContentSize().height / 2 -
                                  infoBkground->getContentSize().height / 3);
    this->addChild(infoBkground, z_infoBkground);

    auto bkground =
        Sprite::createWithTexture(texture->getTextureForKey("bkground_0.png"));
    bkground->setPosition(visibleSize / 2);
    this->addChild(bkground, z_bkground);

    auto info = Sprite::createWithSpriteFrameName("option_option.png");
    info->setPosition(visibleSize.width / 2 -
                          infoBkground->getContentSize().width / 2 +
                          info->getContentSize().width / 2,
                      infoBkground->getPosition().y +
                          infoBkground->getContentSize().height / 2 +
                          (visibleSize.height - infoBkground->getPosition().y -
                           infoBkground->getContentSize().height / 2) /
                              2);
    this->addChild(info, z_info);

    auto return0 = Sprite::createWithSpriteFrameName("return_button0.png");
    auto return1 = Sprite::createWithSpriteFrameName("return_button1.png");
    auto return_button = MenuItemSprite::create(return0, return1, [&](Ref*) {
        this->disableAllButtons();

        auto f = FadeIn::create(MyDefault.act.time);
        auto act = EaseIn::create(f, MyDefault.act.rate);
        auto func = CallFunc::create([this]() {
            this->setOption();
            auto sence = HomePage::createScene();
            Director::getInstance()->replaceScene(sence);
        });
        auto s = Sequence::create(act, func, nullptr);
        blackBkground->runAction(s);
    });
    return_button->setPosition(
        visibleSize.width / 2 + infoBkground->getContentSize().width / 2 -
            return_button->getContentSize().width / 2,
        info->getPosition().y - info->getContentSize().height / 2 +
            return_button->getContentSize().height / 2);
    menu->addChild(return_button);
    this->addButton(return_button);

    showBar = UI_OptionBar::create("ui_optionBar_show_bk.png");
    showBar->setPosition(Vec2((infoBkground->getPosition().x -
                               infoBkground->getContentSize().width / 2) +
                                  showBar->getContentSize().width / 2 +
                                  showBar->getContentSize().width / 12,
                              (infoBkground->getPosition().y +
                               infoBkground->getContentSize().height / 2) -
                                  showBar->getContentSize().height / 2 -
                                  showBar->getContentSize().height / 12));
    showBar->setTitle_X_Add(15);
    showBar->setBK_X_Add(-5);
    //创建子项
    [&]() {
        showBar_highFrame = UI_OptionBarChild_Switch_2Buttons::create(
            XMLManager::getString("option_show_highFrame"),
            "ui_optionBar_switch_bk.png", "ui_optionBar_switch_off.png",
            "ui_optionBar_switch_on.png", 30);
        showBar_particleQuality = UI_OptionBarChild_Switch_3Buttons::create(
            XMLManager::getString("option_show_particleQuality"),
            "ui_optionBar_switch3_bk.png", "ui_optionBar_switch3_low.png",
            "ui_optionBar_switch3_mid.png", "ui_optionBar_switch3_high.png",
            30);
        const vector<base_UI_OptionBarChild*> c{showBar_highFrame,
                                                showBar_particleQuality};
        showBar->addUIChildAndSetPos(
            c, showBar->getContentSize().height * 0.13f, 100);
    }();

    //设置按钮
    [&]() {
        if (MyDefault.option.highFrame) {
            showBar_highFrame->setSwitch(true);
        } else {
            showBar_highFrame->setSwitch(false);
        }

        switch (MyDefault.option.particleQuality) {
            case 0:
                showBar_particleQuality->setSwitch(0);
                break;
            case 1:
                showBar_particleQuality->setSwitch(1);
                break;
            case 2:
                showBar_particleQuality->setSwitch(2);
                break;
        }
    }();
    this->addChild(showBar, z_Button);

    this->disableAllButtons();
    //////////////////////////////////

    return true;
}

void OptionPageLayer0::setOption()
{
    auto runInThread = [&]() {
        auto UD = UserDefault::getInstance();

        UD->setBoolForKey("option_highFrame", MyDefault.option.highFrame);
        UD->setIntegerForKey("option_particleQuality",
                             MyDefault.option.particleQuality);

        UD->flush();
    };

    auto th = thread(runInThread);

    if (MyDefault.option.highFrame != this->showBar_highFrame->getSwitch()) {
        //是高帧率
        if (this->showBar_highFrame->getSwitch()) {
            Director::getInstance()->setAnimationInterval(1 / 60.0f);
            MyDefault.option.highFrame = true;
        } else {
            Director::getInstance()->setAnimationInterval(1 / 30.0f);
            MyDefault.option.highFrame = false;
        }
    }

    MyDefault.option.particleQuality =
        this->showBar_particleQuality->getSwitch();

    th.join();
}
