#pragma once
#include <cocos2d.h>
#include <vector>
#include <algorithm>
#include "LuaCommunicator.h"
#include "Utility\Utility.h"
#include "ObjectProtocol.h"

class ParticleController
{
	ParticleController();
public:
	~ParticleController();
public:
	static ParticleController* Instance()
	{
		static ParticleController* singleton_instance = nullptr;
		if (singleton_instance == nullptr)
			singleton_instance = new ParticleController;

		return singleton_instance;
	}

	unsigned int get_Particle_Cnt()
	{
		return particle_collector.size();
	}

	void init_ParticleController();

	void add_Particle(particle_data* add_particle_data);
	cocos2d::CCParticleSystem* get_Particle(const char* particle_name);
	cocos2d::CCParticleSystem* get_Particle(unsigned int index);
	void update_Particle();
	void destroy_Particle();
	void destroy_ParticleController();
private:
	std::vector<in_particle_data*>particle_collector;
};
