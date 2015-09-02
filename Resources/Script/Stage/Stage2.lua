local stage2 = {}
local isVictorious = false
local isStageOver = false

local GameObject_ID = {}

function stage2.stage_init(GameObject, Map)
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

	add_map(map_table[2])


--	add_object(object_table[2], 700, 100)
	table.insert(GameObject_ID, add_object(object_table[1], 900, 200))

	table.insert(GameObject_ID, add_object(object_table[5], 1500, 150))
	table.insert(GameObject_ID, add_object(object_table[6], 1300, 100))

	table.insert(GameObject_ID, add_object(object_table[5], 2300, 100))
	table.insert(GameObject_ID, add_object(object_table[6], 2000, 50))

	bindUI_on_game_object("skill_button_1", "charge_straight")
	bindUI_on_game_object("skill_button_2", "power_attack")
	bindUI_on_game_object("skill_button_3", "chain_attack")
	bindUI_on_game_object("skill_button_4", "")

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage2.stage_update()
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
