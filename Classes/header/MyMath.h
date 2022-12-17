#ifndef __MY_MATH_H__
#define __MY_MATH_H__

#include "cocos2d.h"
USING_NS_CC;
#include <cmath>

#include "AppDelegate.h"
#include "MyDefault.h"
#include "Random.h"

/*一个数的平方
参数:
->t: 被平方的数
*/
inline float POT(float t) noexcept { return t * t; }

constexpr float PI = 3.1415926f;

//三角函数,角度模式
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
    //获得旋转的角度
    /*
    旋转规则:
    以正右方向为0度,顺时针方向从0~360度

                      270度
                       ↑(y)
                       │
                       |
       180度───┼───→0度(360度)
                       │    (x)
                       │
                      90度


    */
    static float getRotation(const Vec2& from, const Vec2& to);

    //通过此函数来加或减角度可以保证在操作之后仍然保持旋转规则
    static float addRotation(const float nowRotation, const float addRotation);

    /*两点间的距离
    参数:
    ->p1: 第一个点
    ->p2: 第二个点
    */
    static inline float distance(const Vec2& p1, const Vec2& p2);

    /*子弹的路径
    参数:
    ->from: 起始点
    ->to: 终点
    */
    static ccBezierConfig getBezier(const Vec2& from, const Vec2& to);

    /*获得子弹在追踪下一个目标时的路径_与第一个贝塞尔曲线结束时的运动趋势有联系
    参数:
    ->from: 起始点
    ->to: 终点
    ->lastBezier: 上一条曲线
    */
    static ccBezierConfig getBezier(const Vec2& from, const Vec2& to,
                                    const ccBezierConfig& lastBezier);

    /*一条曲线,只是看起来象直线
    参数:
    ->from: 起始点
    ->to: 终点
    */
    static ccBezierConfig getBezier_line(const Vec2& from, const Vec2& to);

    /*第一个贝塞尔曲线结束时的运动趋势有联系_曲线看起来直线,只是与上一个曲线的拐点为圆滑
    参数:
    ->from: 起始点
    ->to: 终点
    ->lastBezier: 上一条曲线
    */
    static ccBezierConfig getBezier_line(const Vec2& from, const Vec2& to,
                                         const ccBezierConfig& lastBezier);

    /*获得贝塞尔曲线的长度(近似值)
    参数:
    ->from: 起始点
    ->bezier: 曲线
    */
    static float getBezierLenth(const Vec2& from, const ccBezierConfig& bezier);

    //获得一条线段上,从起始点开始经过一段长度的坐标
    static Vec2 getPosOnLine(const Vec2& from, const Vec2& to, float distance);

    //判断两个浮点数是否相等
    static inline bool float_equal(float num0, float num1,
                                   float delta = 0.000001f);

    //随机一个坐标,传入的坐标为中心点
    static Vec2 randPos(const Vec2& pos, float min, float max);

    //随机一个坐标,传入的坐标为中心点
    static Vec2 randPos(const Vec2& pos, rand_int& r);

    static bool isInRect(const Vec2& pos, const Rect& rect);

    //四舍五入为整数(小数点后一位)
    static int floatToInt(float f);

    //判断此点是否在一次函数上
    static bool isOnLine(float k, float b, const Vec2& pos);

    //初始化屏幕的矩形
    static void setScreenRect();

    //是否在屏幕内
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