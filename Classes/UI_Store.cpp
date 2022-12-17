#include "header/UI_Store.h"

#include <cstdlib>

#include "header/MyDefault.h"
#include "header/MyMath.h"
#include "header/XMLManager.h"

UI_Store_updateBar_button* UI_Store_updateBar_button::create(
    const string& bkground, const string& cover, const string& bar,
    const string& button0, const string& button1)
{
    auto ui = new (std::nothrow) UI_Store_updateBar_button();
    if (ui) {
        ui->cover = Sprite::createWithSpriteFrameName(cover);
        ui->bkground = Sprite::createWithSpriteFrameName(bkground);
        ui->menu = Menu::create();
        ui->bar = LoadingBar::create(bar, Widget::TextureResType::PLIST);

        auto b0 = Sprite::createWithSpriteFrameName(button0);
        auto b1 = Sprite::createWithSpriteFrameName(button1);

        ui->button = MenuItemSprite::create(b0, b1);

        //必须在控件创建完毕后调用
        ui->init();

        ui->autorelease();
        return ui;
    }
    CC_SAFE_DELETE(ui);
    return nullptr;
}

void UI_Store_updateBar_button::setCallBack(const function<bool(int)>& callBack)
{
    this->callBack = callBack;
}

void UI_Store_updateBar_button::setMaxLevel(int max) { this->maxValue = max; }

int UI_Store_updateBar_button::getValue() { return this->value; }

void UI_Store_updateBar_button::setNowLevel(int value) { this->value = value; }

void UI_Store_updateBar_button::_update()
{
    float percent = (float)value / (float)maxValue * 100;
    this->bar->setPercent(percent);
}

const Vec2& UI_Store_updateBar_button::getButtonPos()
{
    return button->getPosition();
}

void UI_Store_updateBar_button::setPosition(const Vec2& pos)
{
    bkground->setPosition(pos);
    bar->setPosition(pos);
    cover->setPosition(pos);

    this->button->setPosition(
        Vec2(pos.x + bkground->getContentSize().width / 2 +
                 button->getContentSize().width * 2.3f,
             pos.y - bkground->getContentSize().height / 2 +
                 button->getContentSize().height / 2));
}

bool UI_Store_updateBar_button::init()
{
    this->addChild(bkground, 0);
    this->addChild(bar, 1);
    this->addChild(cover, 2);
    this->addChild(menu, 3);

    menu->addChild(button);
    menu->setPosition(Vec2::ZERO);

    button->setCallback([&](Ref*) {
        if (this->callBack(value)) {
            value += 1;
        }
    });

    this->schedule([&](float) { this->_update(); }, "update");

    return true;
}

UI_Store_updateBar_info* UI_Store_updateBar_info::create()
{
    auto ui = new (std::nothrow) UI_Store_updateBar_info();
    if (ui && ui->init()) {
        ui->autorelease();
        return ui;
    }
    CC_SAFE_DELETE(ui);
    return nullptr;
}

void UI_Store_updateBar_info::setPosition_leftTop(const Vec2& pos)
{
    this->pos = pos;
}

void UI_Store_updateBar_info::setLineSpacing(float s) { this->lineSpacing = s; }

void UI_Store_updateBar_info::setMaxLevel(int v) { this->maxValue = v; }

void UI_Store_updateBar_info::setNowLevel(int v) { this->value = v; }

void UI_Store_updateBar_info::setNowPercent(float p) { this->nowPercent = p; }

void UI_Store_updateBar_info::setNextPercrnt(float p) { this->nextPercent = p; }

void UI_Store_updateBar_info::setFirstStr(const string& str)
{
    this->firstStr = str;
}

void UI_Store_updateBar_info::setSecondStr(const string& str)
{
    this->nextStr = str;
}

void UI_Store_updateBar_info::setMoneyCost(int c) { this->moneyCost = c; }

void UI_Store_updateBar_info::setExpLevelCost(int e) { this->expLevelCost = e; }

void UI_Store_updateBar_info::refreshText()
{
    if (richText) {
        richText->removeFromParent();
        richText = nullptr;
    }
    richText = RichText::create();
    richText->setAnchorPoint(Vec2(0, 1));
    this->addChild(richText, 0);

    auto e0 = RichElementText::create(0, Color3B(215, 215, 215), 255, firstStr,
                                      MyDefault.font.bold, 36);
    richText->pushBackElement(e0);

    auto e1 = RichElementText::create(0, Color3B(210, 210, 210), 255, nextStr,
                                      MyDefault.font.medium, 30);
    richText->pushBackElement(e1);

    char str0[32] = "";
    std::snprintf(str0, sizeof(str0), "(%d / %d) ", value, maxValue);
    auto e2 = RichElementText::create(0, Color3B(210, 210, 210), 255, str0,
                                      MyDefault.font.medium, 30);
    richText->pushBackElement(e2);

    char str1[32] = "";
    std::snprintf(str1, sizeof(str1), "+%d%%", MyMath::floatToInt(nowPercent));
    auto e3 = RichElementText::create(0, Color3B(210, 210, 210), 255, str1,
                                      MyDefault.font.medium, 30);
    richText->pushBackElement(e3);

    if (moneyCost >= 0) {
        char str2[32] = "";
        std::snprintf(str2, sizeof(str2), "+%d%%",
                      MyMath::floatToInt(nextPercent));
        auto e4 = RichElementText::create(0, Color3B(20, 215, 0), 255, str2,
                                          MyDefault.font.medium, 30);
        richText->pushBackElement(e4);
    }

    richText->setPosition(pos);
    le0->setPosition(Vec2(pos.x, pos.y - lineSpacing));

    char str3[48] = "";
    if (moneyCost >= 0) {
        std::snprintf(str3, sizeof(str3), "%d%s %d%s", moneyCost,
                      XMLManager::getString("lobby_update_coin").c_str(),
                      expLevelCost,
                      XMLManager::getString("lobby_update_exp").c_str());
    } else {
        std::snprintf(str3, sizeof(str3), "%s",
                      XMLManager::getString("lobby_update_fullLevel").c_str());
        le1->setTextColor(Color4B(200, 200, 50, 255));
    }

    le1->setString(str3);
    le1->setPosition(Vec2(pos.x, pos.y - lineSpacing * 2));
}

bool UI_Store_updateBar_info::init()
{
    /*richText = RichText::create();
    richText->setAnchorPoint(Vec2(0, 1));
    this->addChild(richText, 0);*/

    le0 = Label::createWithTTF(XMLManager::getString("lobby_update_money_cost"),
                               MyDefault.font.medium, 30, Size(250, 0),
                               TextHAlignment::LEFT, TextVAlignment::CENTER);
    le0->setTextColor(Color4B(210, 210, 210, 255));
    le0->setAnchorPoint(Vec2(0, 1));
    this->addChild(le0, 0);

    le1 = Label::createWithTTF("0" + XMLManager::getString("lobby_update_coin"),
                               MyDefault.font.medium, 30, Size(250, 0),
                               TextHAlignment::LEFT, TextVAlignment::CENTER);
    le1->setTextColor(Color4B(210, 210, 210, 255));
    le1->setAnchorPoint(Vec2(0, 1));
    this->addChild(le1, 0);
    return true;
}

//////////////////////////////////////////////////////////////////////////

UI_Store_updateBar* UI_Store_updateBar::create(const string& bk_plist,
                                               updateInfoStruct& infoStruct,
                                               const string& writeInfo)
{
    auto bar = new (std::nothrow) UI_Store_updateBar(infoStruct);
    if (bar && bar->init(bk_plist)) {
        bar->writeInfo = writeInfo;
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    return nullptr;
}

void UI_Store_updateBar::setBKPosition(const Vec2& pos)
{
    this->bkGround->setPosition(pos);
}

void UI_Store_updateBar::setButtonBarPosition(const Vec2& pos)
{
    bar_button->setPosition(pos);
}

void UI_Store_updateBar::setInfoBarPosition(const Vec2& pos_leftTop)
{
    bar_info->setPosition_leftTop(pos_leftTop);
}

void UI_Store_updateBar::createButton(const string& bkground,
                                      const string& cover, const string& bar,
                                      const string& button0,
                                      const string& button1)
{
    this->bar_button = UI_Store_updateBar_button::create(bkground, cover, bar,
                                                         button0, button1);
    bar_button->setCallBack([&](int v) -> bool {
        if (this->updateCallBack(v)) {
            return true;
        }
        return false;
    });
    this->addChild(bar_button, 1);
}

void UI_Store_updateBar::createInfo(float lineSpacing, const string& firstStr,
                                    const string& secondStr)
{
    this->bar_info = UI_Store_updateBar_info::create();
    bar_info->setLineSpacing(lineSpacing);
    bar_info->setFirstStr(firstStr);
    bar_info->setSecondStr(secondStr);

    this->addChild(bar_info, 2);
}

void UI_Store_updateBar::refreshInfo()
{
    bar_info->setMaxLevel(infoStruct.maxLevel);
    bar_info->setNowLevel(infoStruct.nowLevel);
    bar_info->setExpLevelCost(infoStruct.expCost[infoStruct.nowLevel + 1] + 1);
    bar_info->setNowPercent(this->getNowValue());
    bar_info->setNextPercrnt(infoStruct.addValue[infoStruct.nowLevel + 1]);
    bar_info->setMoneyCost(infoStruct.moneyCost[infoStruct.nowLevel + 1]);
    bar_info->refreshText();

    bar_button->setMaxLevel(infoStruct.maxLevel);
    bar_button->setNowLevel(infoStruct.nowLevel);

    // infoStruct.nowValue = this->getNowValue();
}

void UI_Store_updateBar::setUpdateFinishCallBack(
    const function<void()>& callBack)
{
    this->callBack = callBack;
}

float UI_Store_updateBar::getNowValue()
{
    float nowPercent = 0;
    for (int x = 0; x < infoStruct.nowLevel + 1; ++x) {
        nowPercent += infoStruct.addValue[x];
    }
    return nowPercent;
}

const Vec2& UI_Store_updateBar::getButtonPos()
{
    return bar_button->getButtonPos();
}

bool UI_Store_updateBar::init(const string& bk)
{
    this->bkGround = Sprite::createWithSpriteFrameName(bk);
    this->addChild(bkGround, 0);

    return true;
}

bool UI_Store_updateBar::updateCallBack(int)
{
    if (infoStruct.nowLevel != infoStruct.maxLevel &&
        MyDefault.gameState.money -
                infoStruct.moneyCost[infoStruct.nowLevel + 1] >=
            0 &&
        infoStruct.expCost[infoStruct.nowLevel + 1] <=
            MyDefault.gameState.exp_level) {
        infoStruct.nowLevel += 1;
        //写入文件
        auto runInThread = [=]()  //使用传值而不是引用,防止多线程访问出错
        {
            auto UD = UserDefault::getInstance();
            UD->setIntegerForKey(writeInfo.c_str(), infoStruct.nowLevel);
            UD->setIntegerForKey("money", MyDefault.gameState.money);
            UD->flush();
        };
        thread th(runInThread);

        //更新升级信息
        bar_info->setNowLevel(infoStruct.nowLevel);

        float nowPercent = this->getNowValue();
        infoStruct.nowValue = nowPercent;

        bar_info->setNowPercent(nowPercent);
        bar_info->setNextPercrnt(infoStruct.addValue[infoStruct.nowLevel + 1]);
        bar_info->setMoneyCost(infoStruct.moneyCost[infoStruct.nowLevel + 1]);
        bar_info->setExpLevelCost(infoStruct.expCost[infoStruct.nowLevel + 1] +
                                  1);
        bar_info->refreshText();

        //减少钱
        MyDefault.gameState.money -= infoStruct.moneyCost[infoStruct.nowLevel];

        th.join();

        this->callBack();

        return true;
    }

    return false;
}
