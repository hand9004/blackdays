#pragma once
#include "GameGraphicComponent.h"
class Normal : public GameGraphicComponent
{
public:
	Normal(void);
	~Normal(void);
public:
	virtual void Init(main_grap_info& game_obj_info);
	virtual void Update();
	virtual void Destroy();
	virtual void SetPacket(grap_to_obj_packet& src_packet);
};

