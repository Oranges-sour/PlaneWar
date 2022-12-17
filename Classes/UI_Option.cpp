#include "header/UI_Option.h"

#include "header/MyDefault.h"

UI_OptionBar* UI_OptionBar::create(const string& bar)
{
    auto b = new (std::nothrow) UI_OptionBar();
    if (b && b->init()) {
        b->bk = Sprite::createWithSpriteFrameName(bar);
        b->addChild(b->bk, 0);
        b->autorelease();
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void UI_OptionBar::addUIChildAndSetPos(const vector<base_UI_OptionBarChild*>& c,
                                       float w, float s)
{
    this->barChild = c;
    for (auto& it : barChild) {
        it->setPosition(Vec2::ZERO);
        this->addChild(it, 1);
    }
    this->setChildPos(w, s);
}

void UI_OptionBar::setTitle_X_Add(float f) { title_x_add = f; }

void UI_OptionBar::setBK_X_Add(float f) { bk_x_add = f; }

Sprite* UI_OptionBar::getBK() { return bk; }

void UI_OptionBar::setChildPos(float w, float s)
{
    const float top =
        (this->bk->getPosition().y + this->bk->getContentSize().height / 2) - w;
    for (size_t x = 0; x < barChild.size(); ++x) {
        auto c = barChild[x];
        switch (c->getUIType()) {
            case UI_OptionBarChild_Type::ui_switch_2Buttons: {
                dynamic_cast<UI_OptionBarChild_Switch_2Buttons*>(c)
                    ->setPosition(
                        Vec2(this->bk->getPosition().x, top - s * (x + 1)),
                        this->bk->getContentSize(), title_x_add, bk_x_add);
                break;
            }
            case UI_OptionBarChild_Type::ui_switch_3Buttons: {
                dynamic_cast<UI_OptionBarChild_Switch_3Buttons*>(c)
                    ->setPosition(
                        Vec2(this->bk->getPosition().x, top - s * (x + 1)),
                        this->bk->getContentSize(), title_x_add, bk_x_add);
                break;
            }
        }
    }
}

UI_OptionBarChild_Switch_2Buttons* UI_OptionBarChild_Switch_2Buttons::create(
    const string& title, const string& bk, const string& button1,
    const string& button2, float fontSize)
{
    auto b = new (std::nothrow) UI_OptionBarChild_Switch_2Buttons();
    if (b) {
        b->autorelease();
        // init
        [&]() {
            auto s0 = Sprite::createWithSpriteFrameName(bk);
            auto s1 = Sprite::createWithSpriteFrameName(bk);
            b->bkGround = MenuItemSprite::create(s0, s1);
            // b->bkGround->setAnchorPoint(Vec2(0, 0.5f));

            b->title = Label::createWithTTF(
                title, MyDefault.font.normal, fontSize, Size(250, 0),
                TextHAlignment::LEFT, TextVAlignment::CENTER);
            b->title = Label::createWithTTF(
                title, MyDefault.font.normal, fontSize, Size(250, 0),
                TextHAlignment::LEFT, TextVAlignment::CENTER);
            b->title->setAnchorPoint(Vec2(0, 0.5f));

            b->button1 = Sprite::createWithSpriteFrameName(button1);
            b->button2 = Sprite::createWithSpriteFrameName(button2);

            b->menu = Menu::create(b->bkGround, nullptr);
            b->menu->setPosition(Vec2::ZERO);
            b->setSwitch(false);  //默认关闭
        }();
        b->addChild(b->title, 1);
        b->addChild(b->menu, 1);
        b->addChild(b->button1, 2);
        b->addChild(b->button2, 2);
        b->init();
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void UI_OptionBarChild_Switch_2Buttons::setSwitch(bool t)
{
    switch_isOn.setSwitch(t);
    if (t)  //此时开关为开,右边显示,左边隐藏
    {
        this->button1->setVisible(false);
        this->button2->setVisible(true);
    } else {
        this->button1->setVisible(true);
        this->button2->setVisible(false);
    }
}

bool UI_OptionBarChild_Switch_2Buttons::getSwitch()
{
    return switch_isOn.isSet();
}

bool UI_OptionBarChild_Switch_2Buttons::init()
{
    this->bkGround->setCallback(
        [&](Ref*) { this->setSwitch(!this->getSwitch()); });
    return true;
}

void UI_OptionBarChild_Switch_2Buttons::setPosition(const Vec2& p,
                                                    const Size& s,
                                                    float title_x_add,
                                                    float bk_x_add)
{
    float title_x = p.x - s.width / 2 + title_x_add;
    this->title->setPosition(Vec2(title_x, p.y));

    float bk_x = p.x + this->bkGround->getContentSize().width / 2 + bk_x_add;
    this->bkGround->setPosition(Vec2(bk_x, p.y));

    float button1_x = (bk_x - this->bkGround->getContentSize().width / 2) +
                      this->bkGround->getContentSize().width / 2 * 0.5;
    float button2_x = (bk_x - this->bkGround->getContentSize().width / 2) +
                      this->bkGround->getContentSize().width / 2 * 1.5;

    this->button1->setPosition(Vec2(button1_x, p.y));
    this->button2->setPosition(Vec2(button2_x, p.y));
}

UI_OptionBarChild_Switch_3Buttons* UI_OptionBarChild_Switch_3Buttons::create(
    const string& title, const string& bk, const string& button1,
    const string& button2, const string& button3, float fontSize)
{
    auto b = new (std::nothrow) UI_OptionBarChild_Switch_3Buttons();
    if (b) {
        b->autorelease();
        // init
        [&]() {
            auto s0 = Sprite::createWithSpriteFrameName(bk);
            auto s1 = Sprite::createWithSpriteFrameName(bk);
            b->bkGround = MenuItemSprite::create(s0, s1);

            b->title = Label::createWithTTF(
                title, MyDefault.font.normal, fontSize, Size(250, 0),
                TextHAlignment::LEFT, TextVAlignment::CENTER);
            b->title->setAnchorPoint(Vec2(0, 0.5));

            b->button1 = Sprite::createWithSpriteFrameName(button1);
            b->button2 = Sprite::createWithSpriteFrameName(button2);
            b->button3 = Sprite::createWithSpriteFrameName(button3);

            b->menu = Menu::create(b->bkGround, nullptr);
            b->menu->setPosition(Vec2::ZERO);
            b->setSwitch(0);  //默认0
        }();
        b->addChild(b->title, 1);
        b->addChild(b->menu, 1);
        b->addChild(b->button1, 2);
        b->addChild(b->button2, 2);
        b->addChild(b->button3, 2);
        b->init();
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void UI_OptionBarChild_Switch_3Buttons::setSwitch(int t)
{
    if (t > 2) {
        this->switch_on = 2;
    } else if (t < 0) {
        this->switch_on = 0;
    } else {
        this->switch_on = t;
    }

    switch (this->switch_on) {
        case 0: {  // 1-0-0
            this->button1->setVisible(true);
            this->button2->setVisible(false);
            this->button3->setVisible(false);
            break;
        }
        case 1: {  // 0-1-0
            this->button1->setVisible(false);
            this->button2->setVisible(true);
            this->button3->setVisible(false);
            break;
        }
        case 2: {  // 0-0-1
            this->button1->setVisible(false);
            this->button2->setVisible(false);
            this->button3->setVisible(true);
            break;
        }
    }
}

int UI_OptionBarChild_Switch_3Buttons::getSwitch() { return this->switch_on; }

bool UI_OptionBarChild_Switch_3Buttons::init()
{
    this->bkGround->setCallback([&](Ref*) {
        if (this->switch_on + 1 > 2) {
            this->setSwitch(0);
        } else {
            this->setSwitch(this->getSwitch() + 1);
        }
    });
    return true;
}

void UI_OptionBarChild_Switch_3Buttons::setPosition(const Vec2& p,
                                                    const Size& s,
                                                    float title_x_add,
                                                    float bk_x_add)
{
    float title_x = p.x - s.width / 2 + title_x_add;
    this->title->setPosition(Vec2(title_x, p.y));

    float bk_x = p.x + this->bkGround->getContentSize().width / 2 + bk_x_add;
    this->bkGround->setPosition(Vec2(bk_x, p.y));

    float button1_x = (bk_x - this->bkGround->getContentSize().width / 2) +
                      this->bkGround->getContentSize().width / 3 * 0.5;
    float button2_x = (bk_x - this->bkGround->getContentSize().width / 2) +
                      this->bkGround->getContentSize().width / 3 * 1.5;
    float button3_x = (bk_x - this->bkGround->getContentSize().width / 2) +
                      this->bkGround->getContentSize().width / 3 * 2.5;

    this->button1->setPosition(Vec2(button1_x, p.y));
    this->button2->setPosition(Vec2(button2_x, p.y));
    this->button3->setPosition(Vec2(button3_x, p.y));
}
