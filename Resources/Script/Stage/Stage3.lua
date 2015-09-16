local stage3 = {}
local isVictorious = false
local isStageOver = false

local GameObject_ID = {}

local object_table = {}
local map_table = {}

function stage3.stage_init(GameObject, Map)
	object_table = GameObject.getVarString()
	map_table = Map.getVarString()

	for key, value in ipairs(map_table) do
		LOG(value)
	end

	for key, value in ipairs(object_table) do
		LOG(value)
	end

	add_map(map_table[3])

	table.insert(GameObject_ID, add_object(object_table[2], 700, 100))

	table.insert(GameObject_ID, add_object(object_table[1], 600, 150))
	table.insert(GameObject_ID, add_object(object_table[1], 600, 50))
	table.insert(GameObject_ID, add_object(object_table[3], 500, 150))
	table.insert(GameObject_ID, add_object(object_table[3], 500, 50))

	table.insert(GameObject_ID, add_object(object_table[7], 3200, 150))
	table.insert(GameObject_ID, add_object(object_table[7], 3200, 100))
	table.insert(GameObject_ID, add_object(object_table[7], 3200, 50))

	table.insert(GameObject_ID, add_object(object_table[6], 3400, 125))
	table.insert(GameObject_ID, add_object(object_table[6], 3400, 100))
	table.insert(GameObject_ID, add_object(object_table[6], 3400, 75))

	table.insert(GameObject_ID, add_object(object_table[5], 3500, 150))
	table.insert(GameObject_ID, add_object(object_table[5], 3500, 100))
	table.insert(GameObject_ID, add_object(object_table[5], 3500, 50))

	bindUI_on_game_object("skill_button_1", "charge_straight")
	bindUI_on_game_object("skill_button_2", "power_attack")
	bindUI_on_game_object("skill_button_3", "chain_attack")
	bindUI_on_game_object("skill_button_4", "")

	set_ObjectInfo(GameObject_ID[6], "move_speed", 6)
	set_ObjectInfo(GameObject_ID[7], "move_speed", 6)
	set_ObjectInfo(GameObject_ID[8], "move_speed", 6)

	command_to_object(GameObject_ID[2], "Patrol", 3900, 150, 1000)
	command_to_object(GameObject_ID[3], "Patrol", 3900, 50, 1000)
	command_to_object(GameObject_ID[4], "Patrol", 3900, 150, 1000)
	command_to_object(GameObject_ID[5], "Patrol", 3900, 50, 1000)

	command_to_object(GameObject_ID[6], "Patrol", 100, 150, 1000)
	command_to_object(GameObject_ID[7], "Patrol", 100, 100, 1000)
	command_to_object(GameObject_ID[8], "Patrol", 100, 50, 1000)

	command_to_object(GameObject_ID[9], "Patrol", 100, 125, 1000)
	command_to_object(GameObject_ID[10], "Patrol", 100, 100, 1000)
	command_to_object(GameObject_ID[11], "Patrol", 100, 75, 1000)

	command_to_object(GameObject_ID[12], "Patrol", 100, 150, 1000)
	command_to_object(GameObject_ID[13], "Patrol", 100, 100, 1000)
	command_to_object(GameObject_ID[14], "Patrol", 100, 50, 1000)

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage3.stage_update()
	enemy_cnt = get_enemy_cnt()
	ally_cnt = get_ally_cnt()
	isPlayerDead = get_is_object_alive("player")

	if enemy_cnt <= 0 then
		isStageOver = true
		isVictorious = true
	elseif ally_cnt <= 0 then
		isStageOver = true
		isVictorious = false
	elseif isPlayerDead == true then
		isStageOver = true
		isVictorious = false
	end
end

function stage3.stage_destroy()
	stage3 = nil
	isVictorious = nil
	isStageOver = nil

	GameObject_ID = nil
	object_table = nil
	map_table = nil
end

function stage3.isStageOverStat()
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

function stage3.setGameObject_ID_List(list)
	GameObject_ID = list
end

return stage3
