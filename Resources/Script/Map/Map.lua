stage1 = {
			stage_type = "Infiltration",
			{"Images/stage1.plist"},

			{
				index = 0,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 200},

			},

			{
				index = 1,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 200},

				{100, 100, "Images/stage1.plist", "tree1.png", 0},
			},

			{
				index = 2,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 200},

				{100, 180, "Images/stage1.plist", "tree1.png", 0},
			},

			{
				index = 3,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 200},
			},

			{
				index = 4,
				background = "Images/background3.png",
				move_area = {0, 0, 800, 200},
			},

			trigger_data =
			{
				{3850, 0, 150, 210, "stage_clear"},
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
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},

				{100, 200, "Images/stage1.plist", "tree1.png", 0},
				{50, 0, "Images/stage1.plist", "tree.png", 0},
				{200, 100, "Images/stage1.plist", "tree1.png", 0},
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

			trigger_data =
			{
			},

			particle_data =
			{
			},
		 }

stage3 = {
			stage_type = "Battle",
			{"Images/stage1.plist"},

			{
				index = -1,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},
			},

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
