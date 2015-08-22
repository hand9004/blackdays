local stage3 = {}
local isVictorious = false
local isStageOver = false

function stage3.stage_init(GameObject, Map)
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

	add_map(map_table[3])

	add_object(object_table[1], -150, 170)
--	add_object(object_table[1], 100, 130)
--	add_object(object_table[2], -100, 100)
--	add_object(object_table[2], 200, 80)
--	add_object(object_table[2], 400, 50)

	add_object(object_table[3], -400, 150)
	add_object(object_table[4], -300, 100)
	add_object(object_table[3], 1000, 130)
	add_object(object_table[4], 800, 110)
	add_object(object_table[3], 1300, 125)
	add_object(object_table[4], 1000, 90)

--	load_background_music("sounds/main_menu.mp3")
	play_background_music("sounds/main_menu.mp3", true)
end

function stage3.stage_update()
	enemy_cnt = get_enemy_cnt()
	ally_cnt = get_ally_cnt()

	if enemy_cnt <= 0 then
		isStageOver = true
		isVictorious = true
	elseif ally_cnt <= 0 then
		isStageOver = true
		isVictorious = false
	end
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

return stage3
