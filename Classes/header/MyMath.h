#ifndef __MY_MATH_H__
#define __MY_MATH_H__

#include "cocos2d.h"
USING_NS_CC;
#include <cmath>

#include "AppDelegate.h"
#include "MyDefault.h"
#include "Random.h"

/*һ������ƽ��
����:
->t: ��ƽ������
*/
inline float POT(float t) noexcept { return t * t; }

constexpr float PI = 3.1415926f;

//���Ǻ���,�Ƕ�ģʽ
class DEG {
public:
    static float tan(float d) { return tanf(CC_DEGREES_TO_RADIANS(d)); }
    static float sin(float d) { return sinf(CC_DEGREES_TO_RADIANS(d)); }
    static float cos(float d) { return cosf(CC_DEGREES_TO_RADIANS(d)); }

    static float atan(float d) { return CC_RADIANS_TO_DEGREES(atanf(d)); }
    static float asin(float d) { return CC_RADIANS_TO_DEGREES(asinf(d)); }
    static float acos(float d) { return CC_RADIANS_TO_DEGREES(acosf(d)); }
};

class MyMath {
public:
    //�����ת�ĽǶ�
    /*
    ��ת����:
    �����ҷ���Ϊ0��,˳ʱ�뷽���0~360��

                      270��
                       ��(y)
                       ��
                       |
       180�ȩ������੤������0��(360��)
                       ��    (x)
                       ��
                      90��


    */
    static float getRotation(const Vec2& from, const Vec2& to);

    //ͨ���˺������ӻ���Ƕȿ��Ա�֤�ڲ���֮����Ȼ������ת����
    static float addRotation(const float nowRotation, const float addRotation);

    /*�����ľ���
    ����:
    ->p1: ��һ����
    ->p2: �ڶ�����
    */
    static inline float distance(const Vec2& p1, const Vec2& p2);

    /*�ӵ���·��
    ����:
    ->from: ��ʼ��
    ->to: �յ�
    */
    static ccBezierConfig getBezier(const Vec2& from, const Vec2& to);

    /*����ӵ���׷����һ��Ŀ��ʱ��·��_���һ�����������߽���ʱ���˶���������ϵ
    ����:
    ->from: ��ʼ��
    ->to: �յ�
    ->lastBezier: ��һ������
    */
    static ccBezierConfig getBezier(const Vec2& from, const Vec2& to,
                                    const ccBezierConfig& lastBezier);

    /*һ������,ֻ�ǿ�������ֱ��
    ����:
    ->from: ��ʼ��
    ->to: �յ�
    */
    static ccBezierConfig getBezier_line(const Vec2& from, const Vec2& to);

    /*��һ�����������߽���ʱ���˶���������ϵ_���߿�����ֱ��,ֻ������һ�����ߵĹյ�ΪԲ��
    ����:
    ->from: ��ʼ��
    ->to: �յ�
    ->lastBezier: ��һ������
    */
    static ccBezierConfig getBezier_line(const Vec2& from, const Vec2& to,
                                         const ccBezierConfig& lastBezier);

    /*��ñ��������ߵĳ���(����ֵ)
    ����:
    ->from: ��ʼ��
    ->bezier: ����
    */
    static float getBezierLenth(const Vec2& from, const ccBezierConfig& bezier);

    //���һ���߶���,����ʼ�㿪ʼ����һ�γ��ȵ�����
    static Vec2 getPosOnLine(const Vec2& from, const Vec2& to, float distance);

    //�ж������������Ƿ����
    static inline bool float_equal(float num0, float num1,
                                   float delta = 0.000001f);

    //���һ������,���������Ϊ���ĵ�
    static Vec2 randPos(const Vec2& pos, float min, float max);

    //���һ������,���������Ϊ���ĵ�
    static Vec2 randPos(const Vec2& pos, rand_int& r);

    static bool isInRect(const Vec2& pos, const Rect& rect);

    //��������Ϊ����(С�����һλ)
    static int floatToInt(float f);

    //�жϴ˵��Ƿ���һ�κ�����
    static bool isOnLine(float k, float b, const Vec2& pos);

    //��ʼ����Ļ�ľ���
    static void setScreenRect();

    //�Ƿ�����Ļ��
    static bool isInScreenRect(const Vec2& pos);

    static Rect screenRect;
};

inline float MyMath::distance(const Vec2& p1, const Vec2& p2)
{
    return sqrtf(POT(p1.x - p2.x) + POT(p1.y - p2.y));
}

inline bool MyMath::float_equal(float num0, float num1, float delta)
{
    return (abs(num0 - num1) <= delta);
}

inline int MyMath::floatToInt(float f) { return static_cast<int>(f + 0.5f); }

inline bool MyMath::isOnLine(float k, float b, const Vec2& pos)
{
    float cy = pos.x * k + b;
    return abs(cy - pos.y) < 5;
}

inline void MyMath::setScreenRect()
{
    auto director = Director::getInstance();
    screenRect = Rect(0, 0, MyDefault.designResolutionSize.width,
                      MyDefault.designResolutionSize.height);
}

inline bool MyMath::isInScreenRect(const Vec2& pos)
{
    return screenRect.containsPoint(pos);
}

#endif