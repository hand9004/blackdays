#include "UIComponent.h"


UIComponent::UIComponent(void) : id(0), recv_message_by_main(nullptr), isUISelected(false), isSwipeMode(true)
{
	this->setTouchEnabled(true);
}

UIComponent::~UIComponent(void)
{
}