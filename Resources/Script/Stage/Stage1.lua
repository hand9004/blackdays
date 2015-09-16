local stage1 = {}
local isVictorious = false
local isStageOver = false

local GameObject_ID = {}

local object_table = {}
local map_table = {}

local isCommandToObject = false
local isUIIntroduceShowed = false

function stage1.stage_init(GameObject, Map)
	object_table = GameObject.getVarString()
	map_table = Map.getVarString()

	for key, value in ipairs(map_table) do
		LOG(value)
	end

	for key, value in ipairs(object_table) do
		LOG(value)
	end

	add_map(map_table[1])

	table.insert(GameObject_ID, add_object(object_table[2], 200, 200))

	-- Stage 1 of Level 1
	table.insert(GameObject_ID, add_object(object_table[6], 1300, 200))
--	set_ObjectInfo(GameObject_ID[1], "attack_point", 999)
	-- Stage 1 of Level 3

	bindUI_on_game_object("skill_button_1", "charge_straight")
	bindUI_on_game_object("skill_button_2", "power_attack")
	bindUI_on_game_object("skill_button_3", "chain_attack")
	bindUI_on_game_object("skill_button_4", "")
	
--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage1.stage_update()
	local enemy_cnt = get_enemy_cnt()
	local ally_cnt = get_ally_cnt()
	
	local is_stage_start = get_is_stage_start()
	local is_map_scrolling = is_map_scrolled()
	local talk_string = get_current_talk()
	local collided_trigger_str = get_collided_trigger("player")
--	LOG("enemy_cnt = " .. enemy_cnt .. " ally_cnt = " .. ally_cnt)

	if collided_trigger_str == "stage1_clear" then
		isCommandToObject = false
		isStageOver = true
		isVictorious = true
	elseif collided_trigger_str == "tutorial_trap" then
		set_status_on_object("player", object_event.STUN)
	elseif ally_cnt <= 0 then
		isStageOver = true
		isVictorious = false
	elseif is_stage_start == true then
		if isUIIntroduceShowed == false then
			isUIIntroduceShowed = true
			active_layer("tutorial_layer", true)
		end
	end
	
	if touched_id == ui_introduce_img.id then
		if touched_event == 3 then
			active_layer("tutorial_layer", false)
			active_talk_event("stage_1_tutorial", true)
			set_AllObject_Event(object_event.NO_EVENT)
		end
	end
	
	if talk_string == "stage_1_tutorial" then
			local is_talk_start = get_is_talk_start()
			local player_stat = get_status_on_object("player")
			local conversation_index = get_current_conversation()
			local is_target_dead = get_is_object_alive("enemy1")
			
			if conversation_index == 0 or conversation_index == 1 or conversation_index == 4 or
			conversation_index == 8 or conversation_index == 9 then
				if touched_event == 3 and touched_id == stage_talk_message_background.id then
					set_conversation_change(true)
				end
			elseif conversation_index == 2 then
				if is_map_scrolling == true then
					set_conversation_change(true)
				end
			elseif conversation_index == 3 then
				if player_stat == object_event.MOVE then
					set_conversation_change(true)
				end
			elseif conversation_index == 5 then
				if is_target_dead == true then
					set_conversation_change(true)
					active_layer("conversation_layer", true)
				else
					if isCommandToObject == false then
						command_to_object(GameObject_ID[2], "Patrol", 1000, 200, 1500)
						isCommandToObject = true
					end
					active_layer("conversation_layer", false)
				end
			elseif conversation_index == 6 then
				if player_stat == object_event.STUN then
					set_conversation_change(true)
				end
			elseif conversation_index == 7 then
				if player_stat == object_event.SEARCHING_RECOGNIZE_AREA then
					set_conversation_change(true)
				end
			end
	end
	
	touched_id = 0
	touched_sort = 0
	touched_event = 0
	touched_message = 0
end

function stage1.stage_destroy()
	stage1 = nil
	isVictorious = nil
	isStageOver = nil

	GameObject_ID = nil
	object_table = nil
	map_table = nil
end

function stage1.isStageOverStat()
	local stage_over_status = ""

	if isStageOver then
		if isVictorious then
			stage_over_status = "victory"
		else
			stage_over_status = "defeat"
		end
	else
		stage_over_status = "stage_continue"
	end

	return stage_over_status
end

function stage1.setGameObject_ID_List(list)
	GameObject_ID = list
end

return stage1
