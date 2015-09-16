local isStageFinished = false
local stage_reached_max = false
local max_stage_size = 3
local stage_index = 1
local stage_selected = false

local Conversation = {}
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
		copy_script_module_on_android("Stage/", "Conversation")
		copy_script_module_on_android("Object/", "GameObject")
		copy_script_module_on_android("Map/", "Map")

		stage_table = require (ret)
		Conversation = require "Conversation"
		GameObject = require "GameObject"
		Map = require "Map"
	elseif platform_type == "IOS" then

	elseif platform_type == "Windows" then
		ret = "Stage/Stage" .. stage_index

		stage_table = require (ret)
		Conversation = require "Stage/Conversation"
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
		send_message("CHECK_BOX", camera_hold_check_box.id, false)

		local platform_type = get_platform_type()
		StageManager.load_stageModule(platform_type)

		local conv_str = Conversation.getVarString()
		
		if stage_reached_max == false then
			stage_table.stage_init(GameObject, Map)
		end

		stage_message.message = "스테이지 " .. stage_index;

		if stage_message.message == "스테이지 1" then
			stage_explain_message.message = "튜토리얼을 마치세요!"
			set_talk_event(conv_str[1])
		elseif stage_message.message == "스테이지 2" then
			stage_explain_message.message = "적을 피해서 대상을 암살하세요!"
		elseif stage_message.message == "스테이지 3" then
			stage_explain_message.message = "플레이어가 죽지 않은 상태에서\n\n아군 병사를 도와 적을 전멸시키세요!"
		end

		set_active_UI(stage_message.id, true)
		set_active_UI(stage_explain_message.id, true)
		send_message("POPUP", stage_message.id, stage_message.message)
		send_message("POPUP", stage_explain_message.id, stage_explain_message.message)

		stage_reached_max = false
	else
		LOG("Stage Is Over Maximum.")

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
					send_message("POPUP", stage_message.id, "자유를 위해 전진할 때이다!")

					isStageFinished = true
				elseif clear_flag == "defeat" then
					send_stage_flag(true, false)

					set_active_UI(stage_message.id, true)
					send_message("POPUP", stage_message.id, "이 나라의 자유는 죽었다.\n\n여기까지 인가...?")

					isStageFinished = true
				else
					send_stage_flag(false, false)
				end
			else
				send_stage_flag(false, false)

				set_active_UI(stage_message.id, true)
				send_message("POPUP", stage_message.id, "자유는 승리하였고,\n\n새로운 시대의 시작이다!")
				isStageFinished = true
			end
		end
	end
end

function StageManager.destroy_Stage()
	StageManager = nil
	GameObject = nil
	Map = nil

	stage_table.stage_destroy()
	stage_table = nil
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
