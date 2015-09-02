#include "GameGraphicController.h"
#include "ObjectController.h"
#include "../Utility/Utility.h"
#include "Normal.h"
#include "Animate.h"

USING_NS_CC;

#include <algorithm>

void set_animate_sort(std::vector<const char*>&dest_list, obj_info& src_data)
{
	unsigned int src_ani_size = src_data.ani_image_list.size();

	for(unsigned int i = 0; i < src_ani_size; ++i)
	{
		aniframe_info* aniframe_iter = src_data.ani_image_list[i];

		unsigned int res_list_size = dest_list.size();
		if(res_list_size <= 0)
			dest_list.push_back(aniframe_iter->ani_name);
		else
		{
			bool hasAnimationStr = false;
			for(unsigned int j = 0; j < res_list_size; ++j)
			{
				const char* ani_str = dest_list[j];
				if(!strcmp(aniframe_iter->ani_name, ani_str))
				{
					hasAnimationStr = true;
					break;
				}
			}

			if(!hasAnimationStr)
				dest_list.push_back(aniframe_iter->ani_name);
		}
	}
}

GameGraphicController::GameGraphicController(void) : active_component_key(nullptr)
{

}


GameGraphicController::~GameGraphicController(void)
{
}

void GameGraphicController::init()
{
	recog_area_draw_nd = CCDrawNode::create();
	ObjectController::Instance()->addChild(recog_area_draw_nd);
}

void GameGraphicController::add_Object(obj_info& game_obj_info)
{
	// 메인으로 사용되는 Sprite 이미지들을 로드한다.
	std::map<const char*, const char*>::iterator i;
	std::map<const char*, const char*>::iterator begin = game_obj_info.main_image.begin();
	std::map<const char*, const char*>::iterator end = game_obj_info.main_image.end();
	for(i = begin; i != end; ++i)
	{
		cocos2d::CCSpriteBatchNode* bat_img_data = cocos2d::CCSpriteBatchNode::create(i->second);

		ObjectController::Instance()->addChild(bat_img_data);
	}

	// 메인으로 사용되는 고정된 이미지의 프레임을 설정한다.
	unsigned int img_frame_size = game_obj_info.img_frame_list.size();
	for(unsigned int i = 0; i < img_frame_size; ++i)
	{
		main_grap_info grap_info;
		GameGraphicComponent* img_frame_comp = new Normal();
		grap_info.m_imgframe_info = game_obj_info.img_frame_list.at(i);

		img_frame_comp->Init(grap_info);

		game_graphic_list[grap_info.m_imgframe_info->key] = img_frame_comp;
	}

	// 전체 설정 파일 중, 애니메이션의 갯수 및 키를 추려낸다.
	std::vector<const char*>ani_key_sort_list;
	set_animate_sort(ani_key_sort_list, game_obj_info);

	// 위에서 얻은 애니메이션의 갯수 및 키를 통하여, 애니메이션 객체를 재구성한다.
	unsigned int ani_size = ani_key_sort_list.size();
	for(unsigned int i = 0; i < ani_size; ++i)
	{
		main_grap_info ani_grap_info;
		const char* ani_key_str = ani_key_sort_list.at(i);
		GameGraphicComponent* img_frame_comp = new Animate();

		const char* list_key_name = NULL;
		main_ani_info ani_frame_comp;

		ani_frame_comp.ani_frame_data = NULL;
		ani_frame_comp.ani_frame_set = NULL;
		ani_frame_comp.effect_data = NULL;

		// 애니메이션 객체의 경우, 각 애니메이션 마다 프레임 넘기는 시간 및 반복과 반복하지 않는
		// 여부를 따로 설정해야할 경우가 있다. 키 값이 같은 경우에 한해서 설정 값에 포인터를 넘겨준다.
		unsigned int ani_frame_set_size = game_obj_info.ani_frame_set_list.size();
		for(unsigned int j = 0; j < ani_frame_set_size; ++j)
		{
			aniframe_set_info* ani_frame_set_iter = game_obj_info.ani_frame_set_list.at(j);

			if(!strcmp(ani_key_str, ani_frame_set_iter->ani_name))
				ani_frame_comp.ani_frame_set = ani_frame_set_iter;
		}

		// 애니메이션을 구성하는 실제적인 객체 포인터를 넘겨준다.
		// plist 파일 안의 키값을 대조하여, x프레임에 어떤 Key에 해당하는 영역을 임의로 설정해주게 된다.
		// ex) 1프레임 - 1.png(key)
		unsigned int ani_frame_size = game_obj_info.ani_image_list.size();
		for(unsigned int j = 0; j < ani_frame_size; ++j)
		{
			aniframe_info* ani_frame_iter = game_obj_info.ani_image_list.at(j);
			
			if(!strcmp(ani_key_str, ani_frame_iter->ani_name))
			{
				list_key_name = ani_frame_iter->ani_name;

				ani_frame_comp.ani_frame_data = ani_frame_iter;
			}
		}

		unsigned int ani_in_effect_size = game_obj_info.effect_list.size();
		for(unsigned int j = 0; j < ani_in_effect_size; ++j)
		{
			effect_info* effect_iter = game_obj_info.effect_list[j];

			if(!strcmp(ani_key_str, effect_iter->grap_object_key))
				ani_frame_comp.effect_data = effect_iter;
			else
				ani_frame_comp.effect_data = NULL;
		}
		ani_grap_info.m_ani_info = &ani_frame_comp;

		img_frame_comp->Init(ani_grap_info);

		game_graphic_list[list_key_name] = img_frame_comp;
	}
}
void GameGraphicController::update_Object()
{
	CCAssert(active_component_key != nullptr, "Can't Update Object. active_component_key is NULL.");

	auto begin = game_graphic_list.begin();
	auto end = game_graphic_list.end();
	auto i = game_graphic_list.begin();

	for(i = game_graphic_list.begin(); i != end; ++i)
	{			
		if(isGraphicFlip)
			i->second->setSpriteFlipX(true);
		else
			i->second->setSpriteFlipX(false);

		if(!strcmp(i->first, active_component_key))
			i->second->setVisible(true);
		else
			i->second->setVisible(false);

		i->second->setTargetPosition(target_pos);
		i->second->setPosition(draw_pos);
		i->second->Update();
	}

	recog_area_draw_nd->clear();

	float width = recog_area.getMaxX() - recog_area.getMinX();
	float height = recog_area.getMaxY() - recog_area.getMinY();

	CCPoint recog_points[4] =
	{
		CCPoint(recog_area.origin.x, recog_area.origin.y),
		CCPoint(recog_area.origin.x + width, recog_area.origin.y),
		CCPoint(recog_area.origin.x + width, recog_area.origin.y + height),
		CCPoint(recog_area.origin.x, recog_area.origin.y + height),
	};

	ccColor4F border_color = ccc4f(1.f, 1.f, 1.f, 0.25f);
	ccColor4F fill_color = ccc4f(0.5f, 0.5f, 0.5f, 0.3f);

	recog_area_draw_nd->drawPolygon(recog_points, 4, fill_color, 0, border_color);
}
void GameGraphicController::clear_Object()
{
	active_component_key = nullptr;

	auto begin = game_graphic_list.begin();
	auto end = game_graphic_list.end();
	auto i = game_graphic_list.begin();

	for(i = game_graphic_list.begin(); i != end; ++i)
	{
		i->second->Destroy();
		SAFE_DELETE(i->second);
	}

	if (recog_area_draw_nd != nullptr)
		recog_area_draw_nd->removeFromParentAndCleanup(true);

	map_clear(game_graphic_list);
}

void GameGraphicController::setAlphaValue(unsigned char val)
{
	auto begin = game_graphic_list.begin();
	auto end = game_graphic_list.end();
	auto i = game_graphic_list.begin();

	for(i = game_graphic_list.begin(); i != end; ++i)
		i->second->setAlphaValue(val);
}
unsigned char GameGraphicController::getAlphaValue()
{
	unsigned char alpha_val = 0;

	auto begin = game_graphic_list.begin();
	auto end = game_graphic_list.end();
	auto i = game_graphic_list.begin();

	for(i = game_graphic_list.begin(); i != end; ++i)
	{
		if(!strcmp(i->first, active_component_key))
		{
			alpha_val = i->second->getAlphaValue();
			break;
		}
	}

	return alpha_val;
}

cocos2d::CCRect GameGraphicController::get_Touched_Area(cocos2d::CCPoint& touched_pt)
{
	cocos2d::CCRect ret_rect;

	auto begin = game_graphic_list.begin();
	auto end = game_graphic_list.end();
	auto i = game_graphic_list.begin();

	for(i = game_graphic_list.begin(); i != end; ++i)
	{
		cocos2d::CCRect rect_iter = i->second->getContentRect();

		if(rect_iter.containsPoint(touched_pt))
		{
			ret_rect = rect_iter;
			break;
		}
	}

	return ret_rect;
}
cocos2d::CCRect GameGraphicController::get_Active_Area()
{
	cocos2d::CCRect ret_rect;
	if(active_component_key != nullptr)
	{
		auto begin = game_graphic_list.begin();
		auto end = game_graphic_list.end();
		auto i = game_graphic_list.begin();

		for(i = game_graphic_list.begin(); i != end; ++i)
		{
			cocos2d::CCRect rect_iter = i->second->getContentRect();

			if(!strcmp(i->first, active_component_key))
			{
				ret_rect = rect_iter;
				break;
			}
		}
	}
	else
		ret_rect = cocos2d::CCRect(0.0f, 0.0f, 0.0f, 0.0f);

	return ret_rect;
}