local LimeGreen = {50, 205, 50}
local DarkOliveGreen = {85, 107, 47}
local LightSteelBlue = {176, 196, 222}
local LemonChiffon = {255, 250, 205}
local AquaMarine = {127, 255, 212}
local OliveDrab = {107, 142, 35}

skill_button_1 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 500, posY = 0}
skill_button_2 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 575, posY = 0}
skill_button_3 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 650, posY = 0}
skill_button_4 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 725, posY = 0}

stage_message = {id = 0, message = "test", font_name = "font_1.fnt", mode = "toast"
				, posX = 0, posY = 0, delay_time_ms = 3000, font_size = 90, color = LightSteelBlue}

stage_explain_message = {id = 0, message = "", font_name = "font_1.fnt", mode = "toast"
				, posX = 0, posY = 0, delay_time_ms = 3000, font_size = 30, color = LightSteelBlue}

camera_hold_check_box = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png"
				, posX = 0, posY = 0}

local StageManager = {}

function init_on_platform()
	package.path = package.path .. set_resource_path_on_platform()

	local platform_type = get_platform_type()

	if platform_type == "Android" then
		copy_script_module_on_android("Stage/", "StageManager")

		StageManager = require "StageManager"
	elseif platform_type == "IOS" then

	elseif platform_type == "Windows" then
		StageManager = require "Stage/StageManager"
	end
end

function ui_Init()
	local screen_width, screen_height

	screen_width, screen_height = get_screen_size()

	stage_message.posX = screen_width / 2
	stage_message.posY = screen_height / 2

	stage_explain_message.posX = screen_width / 2
	stage_explain_message.posY = stage_message.posY - 50;

	skill_button_1.id = create_UI("skill_button_1", 1)
	skill_button_2.id = create_UI("skill_button_2", 1)
	skill_button_3.id = create_UI("skill_button_3", 1)
	skill_button_4.id = create_UI("skill_button_4", 1)

	camera_hold_check_box.posY = screen_height - 45

--	skill_slide_selector.id = create_UI("skill_slide_selector", 5)
--	object_slide_selector.id = create_UI("object_slide_selector", 5)
	stage_message.id = create_UI("stage_message", 7)
	stage_explain_message.id = create_UI("stage_explain_message", 7)
	camera_hold_check_box.id = create_UI("camera_hold_check_box", 2)

	local notice_string = "Init ID = "

--	LOG(notice_string .. skill_slide_selector.id)
--	LOG(notice_string .. object_slide_selector.id)
	LOG(notice_string .. stage_message.id)
	LOG(notice_string .. stage_explain_message.id)
	LOG(notice_string .. camera_hold_check_box.id)

	set_active_UI(stage_message.id, false)
	set_active_UI(stage_explain_message.id, false)
end

function ui_Update()
	touched_id, touched_sort, touched_event, touched_message = update_UI()
	if camera_hold_check_box.id == touched_id then
		if touched_event == 3 then
			if touched_message == 0 then
				set_Camera_Is_Fixed(0)
				LOG("Camera Fixed is = " .. 0)
			else
				set_Camera_Is_Fixed(1)
				LOG("Camera Fixed is = " .. 1)
			end
		end
	end
end

function game_Init()
	StageManager.init_Stage()
end

function game_Update()
	StageManager.update_Stage()
end

function game_Destroy()

end

function sound_Init()
end
