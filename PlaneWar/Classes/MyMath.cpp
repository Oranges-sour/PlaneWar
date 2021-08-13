#include "header/MyMath.h"

#include "header/Random.h"

USING_NS_CC;

Rect MyMath::screenRect;

float MyMath::getRotation(const Vec2& from, const Vec2& to)
{
    auto newTo = to;
    //防止k无限大
    if (MyMath::float_equal(from.x, to.x, 0.01f)) {
        newTo.x -= 0.5;
    }
    const float x = newTo.x - from.x;
    const float y = newTo.y - from.y;

    const float k = y / x;
    const float angle = abs(DEG::atan(k));

    float rotation = 0.0f;
    //顺时针旋转为正角度
    //旋转度数  0~360(顺时针

    // x轴上方
    if (from.y < newTo.y) {
        //右上
        if (from.x < newTo.x) {
            rotation = 360 - angle;
        } else {
            //左上
            rotation = 180 + angle;
        }
    } else {
        // x轴下方
        //右下
        if (from.x < newTo.x) {
            rotation = angle;
        } else {
            //左下
            rotation = 180 - angle;
        }
    }
    return rotation;
}

float MyMath::addRotation(const float nowRotation, const float addRotation)
{
    if (addRotation > 0) {
        if (nowRotation + addRotation > 360) {  //转的过一圈了,回到0
            return nowRotation + addRotation - 360;
        } else {
            return nowRotation + addRotation;
        }
    } else {
        if (nowRotation + addRotation < 0) {  //转的过一圈了,回到0
            return 360 + nowRotation + addRotation;
        } else {
            return nowRotation + addRotation;
        }
    }
}

ccBezierConfig MyMath::getBezier(const Vec2& from, const Vec2& to)
{
    ccBezierConfig Bezier = {Vec2::ZERO};
    Bezier.endPosition = to;

    /*
    tag的值可以是1或2,是1计算离起点近的点,2计算离起点远的点
    */
    static auto bezierPoint = [](Vec2 from, Vec2 to, int tag) {
        //防止k无限大
        if (MyMath::float_equal(from.x, to.x)) from.x -= 2;
        Vec2 sub0;
        sub0.x = from.x;
        sub0.y = from.y;

        Vec2 new_to = to - sub0;
        float distance0 = 0.0f;
        if (tag == 1) distance0 = MyMath::distance(to, sub0) / 4;
        if (tag == 2) distance0 = MyMath::distance(to, sub0) / 4 * 3;
        float k0;
        k0 = new_to.y / new_to.x;

        Vec2 p0;
        p0.x = distance0 / sqrtf(POT(k0) + 1);
        p0.y = k0 * p0.x;

        Vec2 sub1;
        sub1.x = p0.x;
        sub1.y = p0.y;
        float k1 = 0.0f;
        k1 = -(1 / k0);
        rand_bool rr;

        float distance1 = 0.0f;
        if (tag == 1) {
            rand_float r(distance0 / 6.0f, distance0 / 5.0f);  //范围可能改变
            distance1 = r();
        }
        if (tag == 2) {
            rand_float r(distance0 / 8.0f, distance0 / 7.0f);  //范围可能改变
            distance1 = r();
        }

        Vec2 bezierp;
        bezierp.x = distance1 / sqrtf(POT(k1) + 1);
        bezierp.y = k1 * bezierp.x;
        if (rr()) {
            bezierp.x = -bezierp.x;
            bezierp.y = -bezierp.y;
        }

        if (from.x < to.x)
            return bezierp + sub0 + sub1;
        else
            return bezierp + sub0 - sub1;
    };

    Bezier.controlPoint_1 = bezierPoint(from, to, 1);
    Bezier.controlPoint_2 = bezierPoint(from, to, 2);
    return Bezier;
}

ccBezierConfig MyMath::getBezier(const Vec2& from, const Vec2& to,
                                 const ccBezierConfig& lastBezier)
{
    ccBezierConfig Bezier = {Vec2::ZERO};
    Bezier.endPosition = to;
    Bezier.controlPoint_2 = getBezier(from, to).controlPoint_2;

    static constexpr float m = 1.5f;

    Bezier.controlPoint_1 = Vec2(
        lastBezier.endPosition.x * (1 + m) - (lastBezier.controlPoint_2.x * m),
        lastBezier.endPosition.y * (1 + m) - (lastBezier.controlPoint_2.y * m));

    return Bezier;
}

ccBezierConfig MyMath::getBezier_line(const cocos2d::Vec2& from,
                                      const cocos2d::Vec2& to)
{
    ccBezierConfig result = {Vec2::ZERO};
    result.endPosition = to;

    /*
    tag的值可以是1或2,是1计算离起点近的点,2计算离起点远的点
    */
    static auto bezierPoint = [](Vec2 from, Vec2 to, int tag) {
        if (MyMath::float_equal(from.x, to.x))
            from.x -= 1.0f;  //防止解析式为直线
        Vec2 sub0;
        sub0.x = from.x;
        sub0.y = from.y;

        Vec2 new_to = to - sub0;
        float distance0 = 0.0f;
        if (tag == 1) distance0 = MyMath::distance(to, sub0) / 4;
        if (tag == 2) distance0 = MyMath::distance(to, sub0) / 4 * 3;
        float k0;
        k0 = new_to.y / new_to.x;

        Vec2 p0;

        p0.x = distance0 / sqrtf(POT(k0) + 1);
        if (from.x > to.x) {
            p0.x = -p0.x;
        }

        p0.y = k0 * p0.x;

        return p0 + sub0;
    };

    result.controlPoint_1 = bezierPoint(from, to, 1);
    result.controlPoint_2 = bezierPoint(from, to, 2);
    return result;
}

ccBezierConfig MyMath::getBezier_line(const cocos2d::Vec2& from,
                                      const cocos2d::Vec2& to,
                                      const cocos2d::ccBezierConfig& lastBezier)
{
    ccBezierConfig result = {Vec2::ZERO};
    result.endPosition = to;
    result.controlPoint_2 = getBezier_line(from, to).controlPoint_2;

    static constexpr float m = 2.5f;

    result.controlPoint_1 = Vec2(
        lastBezier.endPosition.x * (1 + m) - (lastBezier.controlPoint_2.x * m),
        lastBezier.endPosition.y * (1 + m) - (lastBezier.controlPoint_2.y * m));

    return result;
}

float MyMath::getBezierLenth(const Vec2& from, const ccBezierConfig& bezier)
{
    auto bezierPoint = [](const Vec2& from, const ccBezierConfig& bezier,
                          float t) -> Vec2 {
        Vec2 p1 = from * powf((1.0f - t), 3);
        Vec2 p2 = bezier.endPosition * t * 3.0f * powf((1.0f - t), 2);
        Vec2 p3 = bezier.controlPoint_1 * powf(t, 2) * 3.0f * (1.0f - t);
        Vec2 p4 = bezier.controlPoint_2 * powf(t, 3);

        return p1 + p2 + p3 + p4;
    };

    float lenth = 0.0f;
    Vec2 lastPoint = from;
    for (int x = 1; x <= 20; ++x) {
        Vec2 p = bezierPoint(from, bezier, static_cast<float>(x) / 20.0f);
        lenth += MyMath::distance(lastPoint, p);
        lastPoint = p;
    }
    return lenth;
}

Vec2 MyMath::getPosOnLine(const Vec2& from, const Vec2& to, float distance)
{
    auto newTo = to;
    //防止k无限大
    if (MyMath::float_equal(from.x, newTo.x, 0.01f)) {
        newTo.x -= 0.5;
    }
    const float k = (newTo.y - from.y) / (newTo.x - from.x);

    float x = distance / sqrt(POT(k) + 1);
    if (from.x > newTo.x) {
        x = -x;
    }
    const float y = k * x;
    return Vec2(from.x + x, from.y + y);
}

Vec2 MyMath::randPos(const Vec2& pos, float min, float max)
{
    auto result = pos;
    rand_float r0(min, max);
    rand_bool r1;

    if (r1()) {
        result.x += r0();
    } else {
        result.x -= r0();
    }

    if (r1()) {
        result.y += r0();
    } else {
        result.y -= r0();
    }

    return result;
}

Vec2 MyMath::randPos(const Vec2& pos, rand_int& r)
{
    auto result = pos;
    rand_bool r1;
    if (r1()) {
        result.x += r();
    } else {
        result.x -= r();
    }
    if (r1()) {
        result.y += r();
    } else {
        result.y -= r();
    }

    return result;
}

bool MyMath::isInRect(const cocos2d::Vec2& pos, const cocos2d::Rect& rect)
{
    return (pos.x > rect.getMinX() && pos.x < rect.getMaxX() &&
            pos.y > rect.getMinY() && pos.y < rect.getMaxY());
}
