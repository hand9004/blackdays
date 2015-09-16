#include "ParticleController.h"

USING_NS_CC;

ParticleController::ParticleController()
{
}

ParticleController::~ParticleController()
{
}

void ParticleController::init_ParticleController()
{
}

void ParticleController::add_Particle(particle_data* add_particle_data)
{
	cocos2d::CCTexture2D* particle_tex = CCTextureCache::sharedTextureCache()->addImage(add_particle_data->texture_path);

	cocos2d::CCParticleSystemQuad* add_particle_sys = nullptr;

	switch (add_particle_data->parti_type)
	{
	case FIRE:
		add_particle_sys = cocos2d::CCParticleFire::create();
		add_particle_sys->setStartColor(cocos2d::ccc4f(1.f, 0.f, 0.f, 1.f));
		add_particle_sys->setEndColor(cocos2d::ccc4f(0.4f, 0.4f, 0.4f, 0.2f));
		break;
	case FIREWORKS:
		add_particle_sys = cocos2d::CCParticleFireworks::create();
		break;
	case SUN:
		add_particle_sys = cocos2d::CCParticleSun::create();
		break;
	case GALAXY:
		add_particle_sys = cocos2d::CCParticleGalaxy::create();
		break;
	case FLOWER:
		add_particle_sys = cocos2d::CCParticleFlower::create();
		break;
	case METEOR:
		add_particle_sys = cocos2d::CCParticleMeteor::create();
		break;
	case SPIRAL:
		add_particle_sys = cocos2d::CCParticleSpiral::create();
		break;
	case EXPLOSION:
		add_particle_sys = cocos2d::CCParticleExplosion::create();
		break;
	case SMOKE:
		add_particle_sys = cocos2d::CCParticleSmoke::create();
		break;
	case SNOW:
		add_particle_sys = cocos2d::CCParticleSnow::create();
		break;
	case RAIN:
		add_particle_sys = cocos2d::CCParticleRain::create();
		add_particle_sys->setStartColor(cocos2d::ccc4f(1.f, 1.f, 1.f, 0.5f));
		add_particle_sys->setEndColor(cocos2d::ccc4f(1.f, 1.f, 1.f, 0.5f));
		break;
	}

	add_particle_sys->setPositionType(cocos2d::tCCPositionType::kCCPositionTypeRelative);
	add_particle_sys->setTexture(particle_tex);
	add_particle_sys->setPosition(add_particle_data->particle_system_pos);
	add_particle_sys->setBlendAdditive(true);
	add_particle_sys->setLife(add_particle_data->life_val);
	add_particle_sys->setEmissionRate(add_particle_data->emission_rate);
	add_particle_sys->setGravity(add_particle_data->gravity_mass);
	add_particle_sys->retain();

	in_particle_data* particle_dat = new in_particle_data;
	particle_dat->particle_sys = add_particle_sys;
	particle_dat->particle_dat = add_particle_data;
	
	particle_collector.push_back(particle_dat);
}
cocos2d::CCParticleSystem* ParticleController::get_Particle(const char* particle_name)
{
	cocos2d::CCParticleSystem* ret_particle = nullptr;

	unsigned int particle_cnt = particle_collector.size();
	for (unsigned int i = 0; i < particle_cnt; ++i)
	{
		auto iter = particle_collector.at(i);

		if (!strcmp(iter->particle_dat->particle_name, particle_name))
			ret_particle = iter->particle_sys;
	}

	return ret_particle;
}

cocos2d::CCParticleSystem* ParticleController::get_Particle(unsigned int index)
{
	cocos2d::CCParticleSystem* ret_particle = nullptr;

	unsigned int particle_cnt = particle_collector.size();
	for (unsigned int i = 0; i < particle_cnt; ++i)
	{
		auto iter = particle_collector.at(i);

		if (i == index)
			ret_particle = iter->particle_sys;
	}

	return ret_particle;
}

void ParticleController::update_Particle()
{

}

void ParticleController::destroy_Particle()
{
	unsigned int particle_size = particle_collector.size();
	for (unsigned int i = 0; i < particle_size; ++i)
	{
		auto iter = particle_collector.at(i);

		SAFE_DELETE(iter->particle_dat);
		SAFE_DELETE(iter);
	}

	vector_clear(particle_collector);
}
void ParticleController::destroy_ParticleController()
{

}