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
        const string& bk_plist /*������*/,
        updateInfoStruct& infoStruct /*�˲�����Ϣ�ᱻ����Ķ�*/,
        const string& writeInfo /*UserDefault ֵ������*/);
    UI_Store_updateBar(updateInfoStruct& infoStruct) : infoStruct(infoStruct) {}

public:
    void setBKPosition(const Vec2& pos);
    void setButtonBarPosition(const Vec2& pos);
    void setInfoBarPosition(const Vec2& pos_leftTop /*���Ͻ�����*/);

    void createButton(const string& bkground /*���ı���*/,
                      const string& cover /*���ĸ���*/,
                      const string& bar /*��*/,
                      const string& button0 /*��ţ̌��*/,
                      const string& button1 /*��ť����*/);

    void createInfo(float lineSpacing /*�м��*/,
                    const string& firstStr /*������_��һ��*/,
                    const string& secondStr /*������_�ڶ���*/);

    //ˢ����Ϣ,һ���ڴ���������
    void refreshInfo();

    //����˻ص�����������,���������ɹ�,��Ҫȥ���½��,����ȵ���ʾ��
    //��Ҫ�ô˻ص��������������߼�,��������������ʾ
    void setUpdateFinishCallBack(const function<void()>& callBack);

    float getNowValue();

    //��ð�ť����
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

//������
class UI_Store_updateBar_button : public Node {
public:
    static UI_Store_updateBar_button* create(
        const string& bkground /*���ı���*/, const string& cover /*���ĸ���*/,
        const string& bar /*��*/, const string& button0 /*��ţ̌��*/,
        const string& button1 /*��ť����*/);

    //���ûص�,����ֵΪtrueʱbar��ֵ(����ȼ�)�Ż�ı�
    //�ص���������int:��ǰ��bar��ֵ
    void setCallBack(const function<bool(int)>& callBack);

    void setMaxLevel(int max);

    int getValue();

    void setNowLevel(int value);

    void _update();

    //���������ť������
    const Vec2& getButtonPos();

public:  //����̳�
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

//������
class UI_Store_updateBar_info : public Node {
public:
    static UI_Store_updateBar_info* create();

    //���Ͻ�����
    void setPosition_leftTop(const Vec2& pos);

    //�м��
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