-- Setting File

--[[ you can use this function to control User Interface
	 create_UI(table_name, UI_TYPE)
	 update_UI() --> return value 3 --> id, ui_sort, message_box
	 setActive_UI(unsigned int dest_ui_id, bool isActivate)
	 DEBUG_UI(char* str) --> insert string value. then u can see the output.
	 --]]

--here's the introducing of function arguments explanation.

--[[ 	1 - button
 		2 - check_box
 		3 - message_box
 		4 - slide_control
		5 - slide_selector
		6 - background		--]]

background1 = {id = 0, background = "Images/background.png", posX = 0, posY = 0}

background2 = {id = 0, background = "Images/main_menu.png", posX = 480, posY = 18}

start_button = {id = 0, untouched = "Images/starter.png", touched = "Images/starter_clicked.png"
				, posX = 530, posY = 310}

credits_button = {id = 0, untouched = "Images/credits.png", touched = "Images/credits_clicked.png"
				, posX = 525, posY = 100}

gallery_button = {id = 0, untouched = "Images/gallery.png", touched = "Images/gallery_clicked.png"
				, posX = 523, posY = 172}

gallery_bg = {id = 0, background = "Images/gallery_bg.png", posX = 0, posY = 0}

setting_button = {id = 0, untouched = "Images/setting.png", touched = "Images/setting_clicked.png"
				, posX = 18, posY = 360}

exit_button = {id = 0, untouched = "Images/exit_un.png", touched = "Images/exit_on.png", posX = 420, posY = 180}

setting_exit_button = {id = 0, untouched = "Images/exit_un.png", touched = "Images/exit_on.png", posX = 420, posY = 180}

exit_button_gal = {id = 0, untouched = "Images/exit_un.png", touched = "Images/exit_on.png", posX = 420, posY = 400}

tutorial_button = {id = 0, untouched = "Images/tutorial.png", touched = "Images/tutorial_clicked.png", posX = 525, posY = 250}

check_box1 = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png"
				, posX = 0, posY = 0}

check_box2 = {id = 0, unchecked = "Images/unchecked.png", checked = "Images/checked.png"
				, posX = 0, posY = 0}

slide_ctrl1 = {id = 0, slide_background = "Images/slider.png", slide_bar = "Images/slider_bar.png"
				, posX = 0, posY = 0}

slide_ctrl2 = {id = 0, slide_background = "Images/slider.png", slide_bar = "Images/slider_bar.png"
				, posX = 0, posY = 0}

setting_message = {id = 0, background = "Images/message_box_empty_chained.png", message = "", posX = 60, posY = 18}

credits_message = {id = 0, background = "Images/credits_message.png", message = "", posX = 200, posY = 18}

tutorial_message = {id = 0, background = "Images/tutorial_message.png", message = "", posX = 30, posY = 18}

tutorial_1 = {id = 0, untouched = "Images/explain_1.png", touched = "Images/explain_1.png", posX = 80, posY = 80}
tutorial_2 = {id = 0, untouched = "Images/explain_2.png", touched = "Images/explain_2.png", posX = 80, posY = 80}
tutorial_3 = {id = 0, untouched = "Images/explain_3.png", touched = "Images/explain_3.png", posX = 80, posY = 80}
tutorial_4 = {id = 0, untouched = "Images/explain_4.png", touched = "Images/explain_4.png", posX = 80, posY = 80}
tutorial_5 = {id = 0, untouched = "Images/explain_5.png", touched = "Images/explain_5.png", posX = 80, posY = 80}
tutorial_6 = {id = 0, untouched = "Images/explain_6.png", touched = "Images/explain_6.png", posX = 80, posY = 80}

function ui_Init()
	background1.id = create_UI("background1", 6)
	background2.id = create_UI("background2", 6)
	setting_message.id = create_UI("setting_message", 6)
	tutorial_message.id = create_UI("tutorial_message", 6)
	credits_message.id = create_UI("credits_message", 3)

	tutorial_1.id = create_UI("tutorial_1", 1)
	tutorial_2.id = create_UI("tutorial_2", 1)
	tutorial_3.id = create_UI("tutorial_3", 1)
	tutorial_4.id = create_UI("tutorial_4", 1)
	tutorial_5.id = create_UI("tutorial_5", 1)
	tutorial_6.id = create_UI("tutorial_6", 1)

	slide_ctrl1.id = create_UI("slide_ctrl1", 4)
	slide_ctrl2.id = create_UI("slide_ctrl2", 4)
	check_box1.id = create_UI("check_box1", 2)
	check_box2.id = create_UI("check_box2", 2)

	start_button.id = create_UI("start_button", 1)
	credits_button.id = create_UI("credits_button", 1)
	setting_button.id = create_UI("setting_button", 1)
	tutorial_button.id = create_UI("tutorial_button", 1)
	gallery_button.id = create_UI("gallery_button", 1)
	exit_button.id = create_UI("exit_button", 1)
	setting_exit_button.id = create_UI("setting_exit_button", 1)

	gallery_bg.id = create_UI("gallery_bg", 6)
	exit_button_gal.id = create_UI("exit_button_gal", 1)

	local notice_string = "Init ID = "

	LOG(notice_string .. background1.id)
	LOG(notice_string .. background2.id)
	LOG(notice_string .. start_button.id)
	LOG(notice_string .. credits_button.id)
	LOG(notice_string .. credits_message.id)
	LOG(notice_string .. setting_button.id)
	LOG(notice_string .. exit_button.id)
	LOG(notice_string .. exit_button_gal.id)
	LOG(notice_string .. setting_exit_button.id)
	LOG(notice_string .. tutorial_button.id)
	LOG(notice_string .. tutorial_message.id)
	LOG(notice_string .. tutorial_1.id)
	LOG(notice_string .. tutorial_2.id)
	LOG(notice_string .. tutorial_3.id)
	LOG(notice_string .. tutorial_4.id)
	LOG(notice_string .. tutorial_5.id)
	LOG(notice_string .. tutorial_6.id)
	LOG(notice_string, gallery_button.id)
	LOG(notice_string, gallery_bg.id)
	LOG(notice_string, check_box1.id)
	LOG(notice_string, check_box2.id)
	LOG(notice_string, slide_ctrl1.id)
	LOG(notice_string, slide_ctrl2.id)
	LOG(notice_string, setting_message.id)

	set_active_UI(setting_message.id, false)
	set_active_UI(credits_message.id, false)
	set_active_UI(tutorial_message.id, false)
	set_active_UI(tutorial_1.id, false)
	set_active_UI(tutorial_2.id, false)
	set_active_UI(tutorial_3.id, false)
	set_active_UI(tutorial_4.id, false)
	set_active_UI(tutorial_5.id, false)
	set_active_UI(tutorial_6.id, false)
	set_active_UI(slide_ctrl1.id, false)
	set_active_UI(slide_ctrl2.id, false)
	set_active_UI(check_box1.id, false)
	set_active_UI(check_box2.id, false)
	set_active_UI(exit_button.id, false)
	set_active_UI(exit_button_gal.id, false)
	set_active_UI(setting_exit_button.id, false)
	set_active_UI(gallery_bg.id, false)

	create_layer("gallery_layer", 0, 0)
	add_UI_to_layer("gallery_layer", gallery_bg.id, 0, 0)
	add_UI_to_layer("gallery_layer", exit_button_gal.id, 750, 430)

	create_layer("setting_layer", 80, 480)
	add_UI_to_layer("setting_layer", setting_message.id, 0, 0)
	add_UI_to_layer("setting_layer", slide_ctrl1.id, 100, 82)
	add_UI_to_layer("setting_layer", slide_ctrl2.id, 100, 17)
	add_UI_to_layer("setting_layer", setting_exit_button.id, 360, 163)
	add_UI_to_layer("setting_layer", check_box1.id, 320, 97)
	add_UI_to_layer("setting_layer", check_box2.id, 320, 32)

	create_layer("tutorial_layer", 30, 18)
	add_UI_to_layer("tutorial_layer", tutorial_message.id, 0, 0)
	add_UI_to_layer("tutorial_layer", tutorial_1.id, 50, 62)
	add_UI_to_layer("tutorial_layer", tutorial_2.id, 50, 62)
	add_UI_to_layer("tutorial_layer", tutorial_3.id, 50, 62)
	add_UI_to_layer("tutorial_layer", tutorial_4.id, 50, 62)
	add_UI_to_layer("tutorial_layer", tutorial_5.id, 50, 62)
	add_UI_to_layer("tutorial_layer", tutorial_6.id, 50, 62)

	send_message("CHECK_BOX", check_box1.id, true)
	send_message("CHECK_BOX", check_box2.id, true)

	send_message("SLIDE_CONTROL", slide_ctrl1.id, 50)
	send_message("SLIDE_CONTROL", slide_ctrl2.id, 50)

	set_background_music_volume(50)
	set_effect_music_volume(50)
end

function ui_Update()
	local touched_id, touched_sort, touched_event, touched_message = update_UI()

	if touched_event == 2 then
		if slide_ctrl1.id == touched_id then
			set_background_music_volume(touched_message)
		elseif slide_ctrl2.id == touched_id then
			set_effect_music_volume(touched_message)
		end
	elseif touched_event == 3 then
		if setting_button.id == touched_id then
			active_layer("setting_layer", true)
			add_action_to_layer("setting_layer", "move_to", 0, -300, 10.0, true)
		elseif setting_exit_button.id == touched_id then
			set_after_layerAction("setting_layer", "Visible", false)
			add_action_to_layer("setting_layer", "move_to", 0, 300, 10.0, true)
		elseif start_button.id == touched_id then
			replace_scene("Select")
		elseif credits_button.id == touched_id then
			set_active_UI(credits_message.id, true)
		elseif tutorial_button.id == touched_id then
			active_layer("tutorial_layer", true)
			set_active_UI(tutorial_message.id, true)
			set_active_UI(tutorial_1.id, true)
			set_active_UI(tutorial_2.id, false)
			set_active_UI(tutorial_3.id, false)
			set_active_UI(tutorial_4.id, false)
			set_active_UI(tutorial_5.id, false)
			set_active_UI(tutorial_6.id, false)

		elseif gallery_button.id == touched_id then
			active_layer("gallery_layer", true)
		elseif exit_button_gal.id == touched_id then
			active_layer("gallery_layer", false)
		elseif check_box1.id == touched_id then
			if touched_message == 0 then
				pause_background_music()
				LOG("pause background")
			else
				resume_background_music()
				LOG("resume background")
			end
		elseif check_box2.id == touched_id then
			if touched_message == 0 then
				pause_all_effect_music()
				LOG("stop effect")
			else
				resume_all_effect_music()
				LOG("resume background")
			end
		elseif tutorial_1.id == touched_id then
			set_active_UI(tutorial_1.id, false)
			set_active_UI(tutorial_2.id, true)
			set_active_UI(tutorial_3.id, false)
			set_active_UI(tutorial_4.id, false)
			set_active_UI(tutorial_5.id, false)
			set_active_UI(tutorial_6.id, false)
		elseif tutorial_2.id == touched_id then
			set_active_UI(tutorial_1.id, false)
			set_active_UI(tutorial_2.id, false)
			set_active_UI(tutorial_3.id, true)
			set_active_UI(tutorial_4.id, false)
			set_active_UI(tutorial_5.id, false)
			set_active_UI(tutorial_6.id, false)
		elseif tutorial_3.id == touched_id then
			set_active_UI(tutorial_1.id, false)
			set_active_UI(tutorial_2.id, false)
			set_active_UI(tutorial_3.id, false)
			set_active_UI(tutorial_4.id, true)
			set_active_UI(tutorial_5.id, false)
			set_active_UI(tutorial_6.id, false)
		elseif tutorial_4.id == touched_id then
			set_active_UI(tutorial_1.id, false)
			set_active_UI(tutorial_2.id, false)
			set_active_UI(tutorial_3.id, false)
			set_active_UI(tutorial_4.id, false)
			set_active_UI(tutorial_5.id, true)
			set_active_UI(tutorial_6.id, false)
		elseif tutorial_5.id == touched_id then
			set_active_UI(tutorial_1.id, false)
			set_active_UI(tutorial_2.id, false)
			set_active_UI(tutorial_3.id, false)
			set_active_UI(tutorial_4.id, false)
			set_active_UI(tutorial_5.id, false)
			set_active_UI(tutorial_6.id, true)
		elseif tutorial_6.id == touched_id then
			active_layer("tutorial_layer", false)
		end
	end
end

function sound_Init()
	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end
