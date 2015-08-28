#pragma once
#include "Effect.h"
#include "ObjectProtocol.h"

USING_NS_CC;

class GameGraphicComponent
{
public:
	GameGraphicComponent(void);
	virtual ~GameGraphicComponent(void);
public:
	virtual void Init(main_grap_info& game_graphic_info) = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
	virtual void SetPacket(grap_to_obj_packet& src_packet) = 0;

	void setSpriteFlipX(bool isFlip)
	{
		CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't setFlipX");
		draw_spr->setFlipX(isFlip);
		grap_effect.setGraphicFlipX(isFlip);
	}
	void setSpriteFlipY(bool isFlip)
	{
		CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't setFlipY");
		draw_spr->setFlipY(isFlip); 
		grap_effect.setGraphicFlipY(isFlip);
	}
	void setTargetPosition(cocos2d::CCPoint& tar_pos) { target_pos = tar_pos; }
	void setPosition(cocos2d::CCPoint& pos) { CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't setPosition"); draw_spr->setPosition(pos); }
	void setVisible(bool isVisible){ CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't setVisible"); draw_spr->setVisible(isVisible); }
	void setAlphaValue(unsigned char value) { CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't setOpacity"); draw_spr->setOpacity(value); }

	void setContentRect(cocos2d::CCRect src_rect){ content_rect = src_rect; }
	unsigned char getAlphaValue() { CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't getOpacity"); return draw_spr->getOpacity(); }
	cocos2d::CCPoint& getPosition() { CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't getPosition"); draw_spr->getPosition(); }
	cocos2d::CCRect getContentRect() { return content_rect; }
	cocos2d::CCSprite* getSprite() { CCAssert(draw_spr != nullptr, "Sprite Is NULL. Can't getSprite"); return draw_spr; }
protected:
	cocos2d::CCPoint target_pos;
	cocos2d::CCSprite* draw_spr;
	cocos2d::CCRect content_rect;
	Effect grap_effect;
};

