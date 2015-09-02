local stage1 = {}
local isVictorious = false
local isStageOver = false

local GameObject_ID = {}

function stage1.stage_init(GameObject, Map)
	local object_table = {}
	local map_table = {}

	object_table = GameObject.getVarString()
	map_table = Map.getVarString()

	for key, value in ipairs(map_table) do
		LOG(value)
	end

	for key, value in ipairs(object_table) do
		LOG(value)
	end

	add_map(map_table[1])

--	table.insert(GameObject_ID, add_object(object_table[2], 200, 200))
--	add_object(object_table[2], 0, 100)

	table.insert(GameObject_ID, add_object(object_table[4], 200, 200))

	-- Stage 1 of Level 1
	table.insert(GameObject_ID, add_object(object_table[6], 1300, 200))
	table.insert(GameObject_ID, add_object(object_table[6], 1300, 20))

	command_to_object(GameObject_ID[2], "Patrol", 1000, 200, 1000)
	command_to_object(GameObject_ID[3], "Patrol", 1000, 20, 2000)
	-- Stage 1 of Level 2
	table.insert(GameObject_ID, add_object(object_table[6], 2100, 140))
	table.insert(GameObject_ID, add_object(object_table[6], 2100, 100))
	table.insert(GameObject_ID, add_object(object_table[6], 2100, 60))
	table.insert(GameObject_ID, add_object(object_table[6], 2100, 20))

	command_to_object(GameObject_ID[4], "Patrol", 1800, 140, 500)
	command_to_object(GameObject_ID[5], "Patrol", 1800, 100, 1000)
	command_to_object(GameObject_ID[6], "Patrol", 1800, 60, 1500)
	command_to_object(GameObject_ID[7], "Patrol", 1800, 20, 2000)

	set_ObjectInfo(GameObject_ID[4], "move_speed", 5)
	set_ObjectInfo(GameObject_ID[5], "move_speed", 5)
	set_ObjectInfo(GameObject_ID[6], "move_speed", 5)
	set_ObjectInfo(GameObject_ID[7], "move_speed", 5)
	-- Stage 1 of Level 3

	for i = 2, table.getn(GameObject_ID) do
		set_ObjectInfo(GameObject_ID[i], "attack_point", 9999)
	end

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage1.stage_update()
	enemy_cnt = get_enemy_cnt()
	ally_cnt = get_ally_cnt()

	collided_trigger_str = get_collided_trigger("player")
--	LOG("enemy_cnt = " .. enemy_cnt .. " ally_cnt = " .. ally_cnt)

	if collided_trigger_str == "stage_clear" then
		isStageOver = true
		isVictorious = true
	elseif ally_cnt <= 0 then
		isStageOver = true
		isVictorious = false
	end
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

return stage1
