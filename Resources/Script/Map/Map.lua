stage1 = {
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

				{400, 150, "Images/stage1.plist", "tree1.png", 0},
				{300, 0, "Images/stage1.plist", "tree.png", 0},
				{600, 100, "Images/stage1.plist", "tree1.png", 0},
			},

			{
				index = 2,
				background = "Images/background1.png",
				move_area = {0, 0, 800, 200},

				{400, 150, "Images/stage1.plist", "tree1.png", 0},
				{300, 0, "Images/stage1.plist", "tree.png", 0},
				{600, 100, "Images/stage1.plist", "tree1.png", 0},
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
				{3800, 0, 200, 200, "stage_clear"},
			},
			-- name, texture_path, sort_of_particle, pos_x, pos_y,
			-- life_val, emission_rate, gravity
			particle_data =
			{
				{"snow_pt", "Images/RainParticle.png", 10, 400, 550,
				 1.5, 400, 0, -600},
				{"snow_pt_1", "Images/RainParticle.png", 10, 1200, 550,
				 1.5, 400, 0, -600},
				{"snow_pt_2", "Images/RainParticle.png", 10, 2000, 550,
				 1.5, 400, 0, -600},
				{"snow_pt_3", "Images/RainParticle.png", 10, 2800, 550,
				 1.5, 400, 0, -600},
				{"snow_pt_4", "Images/RainParticle.png", 10, 3600, 550,
				 1.5, 400, 0, -600},
				{"fire_pt", "Images/RainParticle.png", 0, 1000, 250,
				 0.5, 200, 0, 300},
			},
		 }

stage2 = {
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
