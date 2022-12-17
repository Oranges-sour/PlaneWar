#ifndef UI_OPTION_H
#define UI_OPTION_H

#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <vector>
using namespace std;

#include "Switch.h"

class base_UI_OptionBarChild;

enum class UI_OptionBarChild_Type {
    ui_base,
    ui_switch_2Buttons,
    ui_switch_3Buttons
};

class UI_OptionBar : public Node {
public:
    static UI_OptionBar* create(const string& bar);

    void addUIChildAndSetPos(const vector<base_UI_OptionBarChild*>& c,
                             float w /*��һ���ӿؼ�����bar�����ĳ���*/,
                             float s /*ÿ���ӿؼ��ļ��*/);

    virtual void setPosition(const Vec2& pos) override
    {
        this->bk->setPosition(pos);
    }
    virtual const Size& getContentSize() const override
    {
        return bk->getContentSize();
    }
    virtual const Vec2& getPosition() const override
    {
        return bk->getPosition();
    }

    //���� �ӿؼ���������˾� Bar����˵ľ���
    void setTitle_X_Add(float f);

    //���� �ӿؼ�����˾� Bar����˵ľ���
    void setBK_X_Add(float f);

    Sprite* getBK();

private:
    void setChildPos(float w /*����bar�����ĳ���*/,
                     float s /*ÿ���ӿؼ��ļ��*/);

private:
    Sprite* bk = nullptr;
    vector<base_UI_OptionBarChild*> barChild;

    float title_x_add = 0.0f;
    float bk_x_add = 0.0f;
};

class base_UI_OptionBarChild : public Node {
public:
    virtual UI_OptionBarChild_Type getUIType()
    {
        return UI_OptionBarChild_Type::ui_base;
    }
};

class UI_OptionBarChild_Switch_2Buttons : public base_UI_OptionBarChild {
public:
    static UI_OptionBarChild_Switch_2Buttons* create(const string& title,
                                                     const string& bk,
                                                     const string& button1,
                                                     const string& button2,
                                                     float fontSize);

    void setSwitch(bool t);
    bool getSwitch();

    virtual UI_OptionBarChild_Type getUIType() override
    {
        return UI_OptionBarChild_Type::ui_switch_2Buttons;
    }

    virtual bool init() override;

public:
    // virtual void setPosition(const Vec2&)override;
    void setPosition(const Vec2& p, const Size& s, float title_x_add,
                     float bk_x_add);

private:
    Menu* menu = nullptr;
    MenuItemSprite* bkGround = nullptr;
    Label* title = nullptr;
    Sprite* button1 = nullptr;
    Sprite* button2 = nullptr;

private:
    Switch switch_isOn;
};

class UI_OptionBarChild_Switch_3Buttons : public base_UI_OptionBarChild {
public:
    static UI_OptionBarChild_Switch_3Buttons* create(
        const string& title, const string& bk, const string& button1,
        const string& button2, const string& button3, float fontSize);

    void setSwitch(int t);
    int getSwitch();

    virtual UI_OptionBarChild_Type getUIType() override
    {
        return UI_OptionBarChild_Type::ui_switch_3Buttons;
    }

    virtual bool init() override;

public:
    // virtual void setPosition(const Vec2&)override;
    void setPosition(const Vec2& p, const Size& s, float title_x_add,
                     float bk_x_add);

private:
    Menu* menu = nullptr;
    MenuItemSprite* bkGround = nullptr;
    Label* title = nullptr;
    Sprite* button1 = nullptr;
    Sprite* button2 = nullptr;
    Sprite* button3 = nullptr;

private:
    int switch_on = 0;
};

#endif
