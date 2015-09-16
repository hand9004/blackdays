local stage2 = {}
local isVictorious = false
local isStageOver = false

local GameObject_ID = {}

local object_table = {}
local map_table = {}

function stage2.stage_init(GameObject, Map)
	object_table = GameObject.getVarString()
	map_table = Map.getVarString()

	for key, value in ipairs(map_table) do
		LOG(value)
	end

	for key, value in ipairs(object_table) do
		LOG(value)
	end

	add_map(map_table[2])

--	add_object(object_table[2], 700, 100)
	table.insert(GameObject_ID, add_object(object_table[4], 300, 100))

	table.insert(GameObject_ID, add_object(object_table[5], 1300, 150))
	table.insert(GameObject_ID, add_object(object_table[6], 1300, 50))

	command_to_object(GameObject_ID[2], "Patrol", 1100, 150, 1000)
	command_to_object(GameObject_ID[3], "Patrol", 1100, 50, 2000)

	table.insert(GameObject_ID, add_object(object_table[5], 2300, 100))
	table.insert(GameObject_ID, add_object(object_table[6], 2300, 50))

	table.insert(GameObject_ID, add_object(object_table[7], 3500, 100))

	command_to_object(GameObject_ID[4], "Patrol", 2000, 100, 1500)
	command_to_object(GameObject_ID[5], "Patrol", 2000, 50, 2000)
	command_to_object(GameObject_ID[6], "Patrol", 3000, 100, 2000)

	bindUI_on_game_object("skill_button_1", "n_run")
	bindUI_on_game_object("skill_button_2", "n_hide")
	bindUI_on_game_object("skill_button_3", "n_sleep_target")
	bindUI_on_game_object("skill_button_4", "")

	for i = 2, table.getn(GameObject_ID) do
		set_ObjectInfo(GameObject_ID[i], "attack_point", 9999)
	end

	set_ObjectInfo(GameObject_ID[6], "move_speed", 8)
	set_ObjectInfo(GameObject_ID[6], "max_health", 1)

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage2.stage_update()
	enemy_cnt = get_enemy_cnt()
	ally_cnt = get_ally_cnt()

--	LOG("enemy_cnt = " .. enemy_cnt .. " ally_cnt = " .. ally_cnt)

	isTargetDead = get_is_object_alive("enemy2")
	collided_trigger_str = get_collided_trigger("player")

	if isTargetDead == true then
		isStageOver = true
		isVictorious = true
	elseif ally_cnt <= 0 then
		isStageOver = true
		isVictorious = false
	elseif collided_trigger_str == "st2_enemy_spawn_1" then
		stage_event_message.message = "근방에 숨어있던 적군 복병이 출현했습니다!"
		set_active_UI(stage_event_message.id, true)
		send_message("POPUP", stage_event_message.id, stage_event_message.message)
		table.insert(GameObject_ID, add_object(object_table[5], 2500, 200))
	end
end

function stage2.stage_destroy()
	stage2 = nil
	isVictorious = nil
	isStageOver = nil

	GameObject_ID = nil
	object_table = nil
	map_table = nil
end

function stage2.isStageOverStat()
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

function stage2.setGameObject_ID_List(list)
	GameObject_ID = list
end

return stage2
