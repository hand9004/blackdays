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

	table.insert(GameObject_ID, add_object(object_table[1], 0, 200))
--	add_object(object_table[2], 0, 100)

	table.insert(GameObject_ID, add_object(object_table[3], 1000, 200))
	table.insert(GameObject_ID, add_object(object_table[4], 1000, 0))

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)

	for key, value in ipairs(GameObject_ID) do
		LOG("Object Unique_ID = " .. value)
	end

	command_to_object(GameObject_ID[2], "Patrol", 500, 200)
	command_to_object(GameObject_ID[3], "Patrol", 500, 0)
end

function stage1.stage_update()
	enemy_cnt = get_enemy_cnt()
	ally_cnt = get_ally_cnt()

--	LOG("enemy_cnt = " .. enemy_cnt .. " ally_cnt = " .. ally_cnt)

	if enemy_cnt <= 0 then
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
