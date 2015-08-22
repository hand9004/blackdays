#pragma once

enum UI_TYPE
{
	BUTTON = 0x1,
	CHECK_BOX = 0x2,
	MESSAGE_BOX = 0x3,
	SLIDE_CONTROL = 0x4,
	SLIDE_SELECTOR = 0x05,
	BACKGROUND = 0x06,
	POPUP_MESSAGE = 0x07,
};

enum TouchEvent
{
	TOUCH_NO_EVENT = 0x0,
	TOUCH_BEGIN = 0x1,
	TOUCH_MOVED = 0x2,
	TOUCH_ENDED = 0x3,
};

// Lua -> UI (Init Data)
struct ImageInfoData
{
	char file_name[256];
	cocos2d::CCPoint image_pt;
};

typedef struct ButtonInfoPacket
{
	ImageInfoData image_info, pressed_image_info;
}butt_pack;

typedef struct CheckBoxInfoPacket
{
	ImageInfoData unchecked_image_info, checked_image_info;
}chk_box_pack;

typedef struct MessageInfoPacket
{
	char message[512];
	ImageInfoData background_image_info;
}message_pack;

typedef struct PopupMessageInfoPacket
{
	char message[512], font_name[128], message_mode[16];
	cocos2d::CCPoint message_pt;
	cocos2d::ccColor3B color_val;
	unsigned long delay_time_ms;
	unsigned int font_size;
}pop_message_pack;

typedef struct SlideControlInfoPacket
{
	ImageInfoData background_image_info, slide_bar_info;
}slide_ctrl_pack;

typedef struct SlideSelectorInfoPacket
{
	ImageInfoData background_image_info, current_selected_info, talk_balloon_info;
	bool isVertical;
}slide_sel_pack;

typedef struct BackgroundInfoPacket
{
	ImageInfoData background_image_info;
}bg_pack;

// Lua -> UI (Update Data)
typedef struct PopupMessageUpdatePacket
{
	char message[512];
}pop_msg_update_pack;

// Main -> UI (Update Data)
typedef struct SlideSelectorMessage_Recv
{
	int current_index;
	unsigned int list_size;
}slidesel_msg_recv;

// UI -> Main (Update Data)
typedef struct SlideSelectorMessage_Send
{
	bool isVertical;
	int current_index;
	cocos2d::CCPoint linked_element_pos, slide_direction_vec;
}slidesel_msg_send;

// 비트 세팅 시, FIELD 매크로와 넣고자 하는 데이터와 AND연산
// 비트 데이터 초기화 시, FIELD 매크로를 보수시킨 후, 원래에 AND 연산
const unsigned int UI_ID_FIELD = 0x00000fff;
const unsigned int UI_TYPE_FIELD = 0x0000f000;
const unsigned int UI_TOUCH_EVENT_FIELD = 0x000f0000;
const unsigned int UI_MESSAGE_FIELD = 0xfff00000;

const unsigned int UI_TYPE_SHIFT = 12;
const unsigned int UI_TOUCH_EVENT_SHIFT = 16;
const unsigned int UI_MESSAGE_SHIFT = 20;

const unsigned int UI_ID_BIT = 0xfff;
const unsigned int UI_TYPE_BIT = 0xf;
const unsigned int UI_TOUCH_EVENT_BIT = 0xf;
const unsigned int UI_MESSAGE_BIT = 0xfff;