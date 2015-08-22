local isStageFinished = false
local stage_reached_max = false
local max_stage_size = 3
local stage_index = 1
local stage_selected = false

local StageManager = {}
local GameObject = {}
local Map = {}

local stage_table = {}

function StageManager.load_stageModule(platform_type)
-- Set Module Depend On Platform.
	local ret = nil

	if platform_type == "Android" then
		ret = "Stage" .. stage_index

		copy_script_module_on_android("Stage/", ret)
		copy_script_module_on_android("Object/", "GameObject")
		copy_script_module_on_android("Map/", "Map")

		stage_table = require (ret)
		GameObject = require "GameObject"
		Map = require "Map"
	elseif platform_type == "IOS" then

	elseif platform_type == "Windows" then
		ret = "Stage/Stage" .. stage_index

		stage_table = require (ret)
		GameObject = require "Object/GameObject"
		Map = require "Map/Map"
	end
end

function StageManager.init_Stage()
	local ret_stage_index = 0

	if not(stage_selected) then
		ret_stage_index = recv_stage_index()
		stage_index = ret_stage_index
		stage_selected = true
	end

	if stage_index <= max_stage_size then
		set_Camera_Is_Fixed(0)
		send_message("CHECK_BOX", camera_hold_check_box.id, false)

		bindUI_on_game_object("skill_slide_selector", "skill")
		bindUI_on_game_object("object_slide_selector", "object")

		local platform_type = get_platform_type()
		StageManager.load_stageModule(platform_type)

		if stage_reached_max == false then
			stage_table.stage_init(GameObject, Map)
		end

		stage_message.message = "Stage " .. stage_index;

		set_active_UI(stage_message.id, true)
		send_message("POPUP", stage_message.id, stage_message.message)

		stage_reached_max = false
	else
		LOG("Stage Is Over Maximum.")

		set_active_UI(skill_slide_selector.id, false)
		set_active_UI(object_slide_selector.id, false)
		set_active_UI(stage_message.id, true)
		send_stage_flag(true, true)

		stage_reached_max = true
	end

	isStageFinished = false
	send_stage_AllClear(stage_reached_max)
end

function StageManager.update_Stage()
	if stage_table then
		if isStageFinished == false then
			if stage_reached_max == false then
				stage_table.stage_update()
				local clear_flag = stage_table.isStageOverStat()

				if clear_flag == "victory" then
					send_stage_flag(true, true)
					stage_index = stage_index + 1

					set_active_UI(stage_message.id, true)
					send_message("POPUP", stage_message.id, "Cleared!")

					isStageFinished = true
				elseif clear_flag == "defeat" then
					send_stage_flag(true, false)

					set_active_UI(stage_message.id, true)
					send_message("POPUP", stage_message.id, "Defeated!")

					isStageFinished = true
				else
					send_stage_flag(false, false)
				end
			else
				send_stage_flag(false, false)

				set_active_UI(stage_message.id, true)
				send_message("POPUP", stage_message.id, "All Cleared!")
				isStageFinished = true
			end
		end
	end
end

function StageManager.isStageAllOver()
	return stage_reached_max;
end

function StageManager.getStageIndex()
	return stage_index
end

function StageManager.setStageIndex(index)
	stage_index = index
end

return StageManager
