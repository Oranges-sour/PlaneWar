#ifndef __UI_STORE_H__
#define __UI_STORE_H__

#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "ui/UIRichText.h"
USING_NS_CC;
using namespace cocos2d::ui;

#include <functional>
#include <string>
using namespace std;

#include "MyDefault.h"

class UI_Store_updateBar_button;
class UI_Store_updateBar_info;

typedef _MyDefault::update_info::_base_update updateInfoStruct;

class UI_Store_updateBar : public Node {
public:
    static UI_Store_updateBar* create(
        const string& bk_plist /*主背景*/,
        updateInfoStruct& infoStruct /*此参数信息会被此类改动*/,
        const string& writeInfo /*UserDefault 值的名字*/);
    UI_Store_updateBar(updateInfoStruct& infoStruct) : infoStruct(infoStruct) {}

public:
    void setBKPosition(const Vec2& pos);
    void setButtonBarPosition(const Vec2& pos);
    void setInfoBarPosition(const Vec2& pos_leftTop /*左上角坐标*/);

    void createButton(const string& bkground /*条的背景*/,
                      const string& cover /*条的覆盖*/,
                      const string& bar /*条*/,
                      const string& button0 /*按钮抬起*/,
                      const string& button1 /*按钮按下*/);

    void createInfo(float lineSpacing /*行间距*/,
                    const string& firstStr /*艺术字_第一个*/,
                    const string& secondStr /*艺术字_第二个*/);

    //刷新信息,一般在创建最后调用
    void refreshInfo();

    //如果此回调函数被调用,表明升级成功,需要去更新金币,经验等的显示了
    //不要用此回调函数运行升级逻辑,仅仅用来更新显示
    void setUpdateFinishCallBack(const function<void()>& callBack);

    float getNowValue();

    //获得按钮坐标
    const Vec2& getButtonPos();

    virtual bool init(const string& bk);

private:
    Sprite* bkGround = nullptr;

    bool updateCallBack(int);

private:
    updateInfoStruct& infoStruct;
    string writeInfo;
    function<void()> callBack = []() -> void { return; };

    UI_Store_updateBar_info* bar_info = nullptr;
    UI_Store_updateBar_button* bar_button = nullptr;
};

//附属类
class UI_Store_updateBar_button : public Node {
public:
    static UI_Store_updateBar_button* create(
        const string& bkground /*条的背景*/, const string& cover /*条的覆盖*/,
        const string& bar /*条*/, const string& button0 /*按钮抬起*/,
        const string& button1 /*按钮按下*/);

    //设置回调,返回值为true时bar的值(比如等级)才会改变
    //回调函数参数int:当前的bar的值
    void setCallBack(const function<bool(int)>& callBack);

    void setMaxLevel(int max);

    int getValue();

    void setNowLevel(int value);

    void _update();

    //获得真正按钮的坐标
    const Vec2& getButtonPos();

public:  //父类继承
    virtual void setPosition(const Vec2& pos) override;
    virtual bool init() override;

private:
    Menu* menu = nullptr;
    MenuItemSprite* button = nullptr;
    Sprite* bkground = nullptr;
    Sprite* cover = nullptr;
    LoadingBar* bar = nullptr;

    int value = 0;
    int maxValue = 0;
    function<bool(int)> callBack = [](int) -> bool { return false; };
};

//附属类
class UI_Store_updateBar_info : public Node {
public:
    static UI_Store_updateBar_info* create();

    //左上角坐标
    void setPosition_leftTop(const Vec2& pos);

    //行间距
    void setLineSpacing(float s);

    void setMaxLevel(int v);

    void setNowLevel(int v);

    void setNowPercent(float p);

    void setNextPercrnt(float p);

    void setFirstStr(const string& str);

    void setSecondStr(const string& str);

    void setMoneyCost(int c);

    void setExpLevelCost(int e);

    void refreshText();

    virtual bool init() override;

public:
    Vec2 pos;
    float lineSpacing = 0.0f;
    int maxValue = 0;
    int value = 0;
    float nowPercent = 0.0f;
    float nextPercent = 0.0f;
    string firstStr = "";
    string nextStr = "";
    int moneyCost = 0;
    int expLevelCost = 0;
    RichText* richText = nullptr;
    Label* le0 = nullptr;
    Label* le1 = nullptr;
};

#endif