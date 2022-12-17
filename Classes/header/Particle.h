#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "ObjectPool.h"
#include "basic_Object.h"

class basic_Particle : public basic_Object//不要使用此类创建任何实例
{
public:
	virtual void updateParticle() = 0;
protected:
	basic_Particle() {}
	~basic_Particle() {}
	virtual bool initParticle(float liveTime, float rate) = 0;
};

class ParticleOnMove : public basic_Particle
{
public:
	static ParticleOnMove* create(const string& parFrame, float liveTime, float rate = (1.0F));

	void KillParticle(float);
	virtual void updateParticle()override;
	virtual void setAddPos(const Vec2& addPos);
protected:
	virtual bool initParticle(float liveTime, float rate)override;

	float addY = 0;
	float addX = 0;
	int addRotate = 0;
};

class ParticleOnExplode : public ParticleOnMove
{
public:
	static ParticleOnExplode* create(const string& parFrame, float liveTime, float rate = (1.0F));
	virtual void updateParticle()override;
protected:
	virtual bool initParticle(float liveTime,float rate)override;
};
#endif