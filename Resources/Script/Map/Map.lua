stage1 = {
			stage_type = "Tutorial",
			{"Images/stage1.plist"},

			{
				index = 0,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 210},

			},

			{
				index = 1,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 210},

				{100, 100, "Images/stage1.plist", "tree1.png", 0},
			},

			{
				index = 2,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 210},

				{100, 180, "Images/stage1.plist", "tree1.png", 0},
			},

			trigger_data =
			{
				{2250, 0, 150, 210, "stage_clear", "stage1_clear"},
				{1600, 0, 150, 210, "trap", "tutorial_trap"},
			},
			-- name, texture_path, sort_of_particle, pos_x, pos_y,
			-- life_val, emission_rate, gravity
			particle_data =
			{
				{"fire_pt", "Images/RainParticle.png", 0, 1000, 250,
				 0.5, 200, 0, 300},
			},
		 }

stage2 = {
			stage_type = "Assassinate",
			{"Images/stage1.plist"},

			{
				index = 0,
				background = "Images/background4.jpg",
				move_area = {0, 0, 900, 240},
			},

			{
				index = 1,
				background = "Images/background4.jpg",
				move_area = {0, 0, 900, 240},

				{100, 200, "Images/stage1.plist", "tree1.png", 0},
				{50, 0, "Images/stage1.plist", "tree.png", 0},
				{200, 100, "Images/stage1.plist", "tree1.png", 0},
			},

			{
				index = 2,
				background = "Images/background4.jpg",
				move_area = {0, 0, 900, 240},
			},

			{
				index = 3,
				background = "Images/background4.jpg",
				move_area = {0, 0, 900, 240},

				{100, 200, "Images/stage1.plist", "tree1.png", 0},
			},

			trigger_data =
			{
				{2600, 180, 100, 60, "spawn", "st2_enemy_spawn_1"},
				{800, 180, 100, 60, "trap", "st2_crowd_control_1"},
			},

			particle_data =
			{
			},
		 }

stage3 = {
			stage_type = "Battle",
			{"Images/stage1.plist"},

			{
				index = 0,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},

			{
				index = 1,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},

			{
				index = 2,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},


			{
				index = 3,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},


			{
				index = 4,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},

			trigger_data =
			{
			},

			particle_data =
			{
			},
		 }

local Map = {}

function Map.getVarString()
	return {"stage1", "stage2", "stage3"}
end

return Map
