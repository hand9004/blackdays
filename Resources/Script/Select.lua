-- Setting File

--[[ you can use this function to control User Interface
	 create_UI(table_name, UI_TYPE)
	 update_UI() --> return value 3 --> id, ui_sort, message_box
	 setActive_UI(unsigned int dest_ui_id, bool isActivate)
	 LOG(char* str) --> insert string value. then u can see the output.
	 --]]

--here's the introducing of function arguments explanation.

--[[ 	1 - button
 		2 - check_box
 		3 - message_box
 		4 - slide_control
		5 - slide_selector
		6 - background		--]]

worldmap_background = {id = 0, background = "Images/worldmap.png", posX = 0, posY = 0}

stage1_marker = {id = 0, untouched = "Images/marker.png", touched = "Images/marker.png"
				, posX = 0, posY = 0}

stage2_marker = {id = 0, untouched = "Images/marker.png", touched = "Images/marker.png"
				, posX = 0, posY = 0}

stage3_marker = {id = 0, untouched = "Images/marker.png", touched = "Images/marker.png"
				, posX = 0, posY = 0}

function ui_Init()
	worldmap_background.id = create_UI("worldmap_background", 6)
	stage1_marker.id = create_UI("stage1_marker", 1)
	stage2_marker.id = create_UI("stage2_marker", 1)
	stage3_marker.id = create_UI("stage3_marker", 1)

	local notice_string = "Init ID = "

	LOG(notice_string .. worldmap_background.id)
	LOG(notice_string .. stage1_marker.id)
	LOG(notice_string .. stage2_marker.id)
	LOG(notice_string .. stage3_marker.id)

--	set_active_UI(stage2_marker.id, false)
--	set_active_UI(stage3_marker.id, false)

	create_layer("select_layer", 0, 0)
	add_UI_to_layer("select_layer", worldmap_background.id, 0, 0)
	add_UI_to_layer("select_layer", stage1_marker.id, 470, 512)
	add_UI_to_layer("select_layer", stage2_marker.id, 580, 400)
	add_UI_to_layer("select_layer", stage3_marker.id, 800, 600)

	add_action_to_layer("select_layer", "scroll", "all", 15.0)
end

function ui_Update()
	touched_id, touched_sort, touched_event, touched_message = update_UI()

	if touched_event == 3 then
		if stage1_marker.id == touched_id then
			ui_Destroy()
			send_stage_index(1)
			replace_scene("Game")
		elseif stage2_marker.id == touched_id then
			ui_Destroy()
			send_stage_index(2)
			replace_scene("Game")
		elseif stage3_marker.id == touched_id then
			ui_Destroy()
			send_stage_index(3)
			replace_scene("Game")
		end
	end
end

function ui_Destroy()
	worldmap_background = nil
	stage1_marker = nil
	stage2_marker = nil
	stage3_marker = nil
end

function sound_Init()

end
