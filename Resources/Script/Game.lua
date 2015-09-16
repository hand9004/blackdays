local LimeGreen = {50, 205, 50}
local DarkOliveGreen = {85, 107, 47}
local LightSteelBlue = {176, 196, 222}
local LemonChiffon = {255, 250, 205}
local AquaMarine = {127, 255, 212}
local OliveDrab = {107, 142, 35}

skill_button_1 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 0, posY = 0}
skill_button_2 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 0, posY = 0}
skill_button_3 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 0, posY = 0}
skill_button_4 = {id = 0, untouched = "Images/button.png", touched = "Images/button.png", posX = 0, posY = 0}

pause_button = {id = 0, untouched = "Images/pause_button.png", touched = "Images/pause_button.png", posX = 0, posY = 0}

pause_background = {id = 0, background = "Images/pause_background.png", posX = 0, posY = 0}
pause_bg_music_check_box = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png", posX = 0, posY = 0}
pause_eff_music_check_box = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png", posX = 0, posY = 0}
pause_vibration_check_box = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png", posX = 0, posY = 0}
pause_back_main_button = {id = 0, untouched = "Images/pause_main_button.png", touched = "Images/pause_main_button.png", posX = 0, posY = 0}
pause_close_button = {id = 0, untouched = "Images/pause_close_button.png", touched = "Images/pause_close_button.png", posX = 0, posY = 0}

stage_message = {id = 0, message = "", font_name = "nanum_font.ttf", mode = "toast"
				, posX = 0, posY = 0, delay_time_ms = 3000, font_size = 50, color = LemonChiffon}

stage_explain_message = {id = 0, message = "", font_name = "nanum_font.ttf", mode = "toast"
				, posX = 0, posY = 0, delay_time_ms = 3000, font_size = 30, color = LemonChiffon}

stage_event_message = {id = 0, message = "", font_name = "nanum_font.ttf", mode = "toast"
				, posX = 0, posY = 0, delay_time_ms = 1000, font_size = 30, color = LemonChiffon}

ui_introduce_img = {id = 0, untouched = "Images/ui_introduce.png", touched = "Images/ui_introduce.png", posX = 0, posY = 0}

stage_talk_indicate_message = {id = 0, message = "대화창을 터치하세요.", font_name = "nanum_font.ttf", mode = "normal"
				, posX = 0, posY = 0, delay_time_ms = 0, font_size = 30, color = DarkOliveGreen}
stage_talk_message = {id = 0, message = "", font_name = "nanum_font.ttf", mode = "normal"
				, posX = 0, posY = 0, delay_time_ms = 0, font_size = 30, color = DarkOliveGreen}
stage_talk_left_img = {id = 0, background = "Images/win_arc00_0.png", posX = 0, posY = 0}
stage_talk_right_img = {id = 0, background = "Images/CloseSelected.png", posX = 0, posY = 0}				
stage_talk_message_background = {id = 0, untouched = "Images/dialog.png", touched = "Images/dialog.png", posX = 0, posY = 0}

camera_hold_check_box = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png"
				, posX = 0, posY = 0}
				
touched_id = 0
touched_sort = 0
touched_event = 0
touched_message = 0

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

	pause_button.posX = screen_width - 75
	pause_button.posY = screen_height - 75

	stage_message.posX = screen_width / 2
	stage_message.posY = screen_height / 2 + stage_message.font_size;

	stage_explain_message.posX = screen_width / 2
	stage_explain_message.posY = stage_message.posY - stage_message.font_size * 2;

	stage_event_message.posX = screen_width / 2
	stage_event_message.posY = screen_height / 2

	skill_button_4.posX = screen_width - 75;
	skill_button_3.posX = skill_button_4.posX - 75;
	skill_button_2.posX = skill_button_3.posX - 75;
	skill_button_1.posX = skill_button_2.posX - 75;

	skill_button_1.id = create_UI("skill_button_1", 1)
	skill_button_2.id = create_UI("skill_button_2", 1)
	skill_button_3.id = create_UI("skill_button_3", 1)
	skill_button_4.id = create_UI("skill_button_4", 1)

	pause_button.id = create_UI("pause_button", 1)
	pause_background.id = create_UI("pause_background", 6)
	pause_bg_music_check_box.id = create_UI("pause_bg_music_check_box", 2)
	pause_eff_music_check_box.id = create_UI("pause_eff_music_check_box", 2)
	pause_vibration_check_box.id = create_UI("pause_vibration_check_box", 2)
	pause_back_main_button.id = create_UI("pause_back_main_button", 1)
	pause_close_button.id = create_UI("pause_close_button", 1)

	stage_message.id = create_UI("stage_message", 7)
	stage_explain_message.id = create_UI("stage_explain_message", 7)
	stage_event_message.id = create_UI("stage_event_message", 7)
	camera_hold_check_box.id = create_UI("camera_hold_check_box", 2)
	
	ui_introduce_img.id = create_UI("ui_introduce_img", 1)
	stage_talk_message_background.id = create_UI("stage_talk_message_background", 1)
	stage_talk_left_img.id = create_UI("stage_talk_left_img", 6)
	stage_talk_right_img.id = create_UI("stage_talk_right_img", 6)		
	stage_talk_indicate_message.id = create_UI("stage_talk_indicate_message", 7)
	stage_talk_message.id = create_UI("stage_talk_message", 7)
	
	create_layer("pause_layer", (screen_width / 2) - 146, (screen_height / 2) - 206)
	add_UI_to_layer("pause_layer", pause_background.id, 0, 0)
	add_UI_to_layer("pause_layer", pause_bg_music_check_box.id, 190, 323)
	add_UI_to_layer("pause_layer", pause_eff_music_check_box.id, 190, 259)
	add_UI_to_layer("pause_layer", pause_vibration_check_box.id, 190, 195)
	add_UI_to_layer("pause_layer", pause_back_main_button.id, 40, 117)
	add_UI_to_layer("pause_layer", pause_close_button.id, 40, 50)

	create_layer("tutorial_layer", 0, 0)
	add_UI_to_layer("tutorial_layer", ui_introduce_img.id, 0, 0)
	
	create_layer("conversation_layer", 0, 0)
	add_UI_to_layer("conversation_layer", stage_talk_message.id, 400, 50)
	add_UI_to_layer("conversation_layer", stage_talk_left_img.id, 0, 0)
	add_UI_to_layer("conversation_layer", stage_talk_right_img.id, 600, 0)
	add_UI_to_layer("conversation_layer", stage_talk_message_background.id, 0, 0)
	
	add_progress_timer(skill_button_1.id, "Radial", "Images/skill_progress_img.png")
	add_progress_timer(skill_button_2.id, "Radial", "Images/skill_progress_img.png")
	add_progress_timer(skill_button_3.id, "Radial", "Images/skill_progress_img.png")
	add_progress_timer(skill_button_4.id, "Radial", "Images/skill_progress_img.png")

	local notice_string = "Init ID = "

	LOG(notice_string .. stage_message.id)
	LOG(notice_string .. stage_explain_message.id)
	LOG(notice_string .. camera_hold_check_box.id)

	active_layer("pause_layer", false)
	active_layer("conversation_layer", false)
	active_layer("tutorial_layer", false)

	set_active_UI(stage_message.id, false)
	set_active_UI(stage_explain_message.id, false)
	set_active_UI(stage_event_message.id, false)
	set_active_UI(stage_talk_indicate_message.id, false)
end

function ui_Update()
	touched_id, touched_sort, touched_event, touched_message = update_UI()
	if touched_event == 3 then
		if camera_hold_check_box.id == touched_id then
				if touched_message == 0 then
					set_camera_is_fixed(false)
				else
					set_camera_is_fixed(true)
				end
				LOG("camera_hold_box_checked")
		elseif pause_button.id == touched_id then
			active_layer("pause_layer", true)
			control_scheduler("Game", true)
			LOG("Pause Button Clicked.")
		elseif pause_bg_music_check_box.id == touched_id then
			if touched_message == 0 then
					pause_background_music()
					LOG("pause background")
				else
					resume_background_music()
					LOG("resume background")
				end
		elseif pause_eff_music_check_box.id == touched_id then
			if touched_message == 0 then
				pause_all_effect_music()
				LOG("stop effect")
			else
				resume_all_effect_music()
				LOG("resume background")
			end
--		elseif pause_vibration_check_box.id == touched_id then
		elseif pause_back_main_button.id == touched_id then
			game_Destroy()
			control_scheduler("Game", false)
			replace_scene("Main")
		elseif pause_close_button.id == touched_id then
			active_layer("pause_layer", false)
			control_scheduler("Game", false)
			LOG("Back Main Button Clicked.")
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
	skill_button_1 = nil
	skill_button_2 = nil
	skill_button_3 = nil
	skill_button_4 = nil
	pause_button = nil

	pause_background = nil
	pause_bg_music_check_box = nil
	pause_eff_music_check_box = nil
	pause_vibration_check_box = nil
	pause_back_main_button = nil
	pause_close_button = nil

	stage_message = nil

	stage_explain_message = nil

	camera_hold_check_box = nil
	StageManager.destroy_Stage()
	StageManager = nil
end

function sound_Init()
end
