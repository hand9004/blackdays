--[[ info {name, atk_pt, def_pt, hp, recognize_area, attack_range_X, attack_range_Y, partol_delay_time(ms)
			move_speed, isEnemy, isHero, isControllable, isAreaAttack} ]]--

player = {info = {"player", 20, 5, 400, 300, 70, 10, 5000, 4.0, false, true, true, false},

			main_image = {"Characters/len.png"},

			image_frame = {},

			ani_image_frame = {
								{
									"Characters/len.png",
									"Characters/len.plist",
									"stand",

									{ "ren0000.png", 1 },
									{ "ren0001.png", 2 },
									{ "ren0002.png", 3 },
									{ "ren0003.png", 4 },
									{ "ren0004.png", 5 },
									{ "ren0005.png", 6 },
									{ "ren0006.png", 7 },
									{ "ren0007.png", 8 },
									{ "ren0008.png", 9 },
									{ "ren0009.png", 10 },
									{ "ren0010.png", 11 },
									{ "ren0011.png", 12 },
									{ "ren0012.png", 13 },
									{ "ren0013.png", 14 },
									{ "ren0014.png", 15 },
									{ "ren0015.png", 16 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"move",

									{ "ren1300.png", 1 },
									{ "ren1301.png", 2 },
									{ "ren1302.png", 3 },
									{ "ren1303.png", 4 },
									{ "ren1304.png", 5 },
									{ "ren1305.png", 6 },
									{ "ren1306.png", 7 },
									{ "ren1307.png", 8 },
									{ "ren1308.png", 9 },
									{ "ren1309.png", 10 },
									{ "ren1310.png", 11 },
									{ "ren1311.png", 12 },
									{ "ren1312.png", 13 },
									{ "ren1313.png", 14 },
									{ "ren1314.png", 15 },
									{ "ren1315.png", 16 },
									{ "ren1316.png", 17 },
									{ "ren1317.png", 18 },
									{ "ren1318.png", 19 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"attack",

									{ "ren5200.png", 1 },
									{ "ren5201.png", 2 },
									{ "ren5202.png", 3 },
									{ "ren5203.png", 4 },
									{ "ren5204.png", 5 },
									{ "ren5205.png", 6 },
									{ "ren5206.png", 7 },
									{ "ren5207.png", 8 },
									{ "ren5208.png", 9 },
									{ "ren5209.png", 10 },
									{ "ren5210.png", 11 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"dead",

									{ "ren2409.png", 1 },
									{ "ren2410.png", 2 },
									{ "ren2411.png", 3 },
									{ "ren2412.png", 4 },
									{ "ren2413.png", 5 },
									{ "ren2414.png", 6 },
									{ "ren2415.png", 7 },
									{ "ren2416.png", 8 },
									{ "ren2417.png", 9 },
									{ "ren2418.png", 10 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"charge",

									{ "ren60v67.png", 1 },
									{ "ren60v68.png", 2 },
									{ "ren60v69.png", 3 },
									{ "ren60v70.png", 4 },
									{ "ren60v71.png", 5 },
									{ "ren60v72.png", 6 },
									{ "ren60v73.png", 7 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"power",

									{ "ren9100.png", 1 },
									{ "ren9101.png", 2 },
									{ "ren9102.png", 3 },
									{ "ren9103.png", 4 },
									{ "ren9104.png", 5 },
									{ "ren9105.png", 6 },
									{ "ren9106.png", 7 },
									{ "ren9107.png", 8 },
									{ "ren9108.png", 9 },
									{ "ren9109.png", 10 },
									{ "ren9110.png", 11 },
									{ "ren9111.png", 12 },
									{ "ren9112.png", 13 },
									{ "ren9113.png", 14 },
									{ "ren9114.png", 15 },
									{ "ren9115.png", 16 },
									{ "ren9116.png", 17 },
									{ "ren9117.png", 18 },
									{ "ren9118.png", 19 },
								},

								{
									"Characters/len.png",
									"Characters/len.plist",
									"chain",

									{ "ren3900.png", 1 },
									{ "ren3901.png", 2 },
									{ "ren3902.png", 3 },
									{ "ren3903.png", 4 },
									{ "ren3904.png", 5 },
									{ "ren3905.png", 6 },
									{ "ren3906.png", 7 },
									{ "ren3907.png", 8 },
									{ "ren3908.png", 9 },
									{ "ren3909.png", 10 },
									{ "ren3910.png", 11 },
									{ "ren3911.png", 12 },
									{ "ren3912.png", 13 },
									{ "ren3913.png", 14 },
									{ "ren3914.png", 15 },
									{ "ren3915.png", 16 },
									{ "ren3916.png", 17 },
									{ "ren3917.png", 18 },
									{ "ren3918.png", 19 },
									{ "ren3919.png", 20 },
								},
							  },

			ani_frame_set = {
								{"stand", 0.02, 0, 0, false},
								{"move", 0.02, 4, 11, false},
								{"attack", 0.04, 0, 0, false},
								{"dead", 0.02, 0, 0, true},
								{"charge", 0.02, 4, 7, false},
								{"power", 0.04, 0, 0, false},
								{"chain", 0.04, 0, 0, false},
							},
			effect_set = {
						 },
			skill = {
						{
							"Images/skill_image_1.png",
							"charge",
							"charge_straight",

							"Melee",
							"Charge",

							charge_dist = 500,
							power_percent = 1.2,
							chain_hit_frame = {},
							particle_data = {},
						},
						{
							"Images/skill_image_2.png",
							"power",
							"power_attack",

							"Melee",
							"Power",

							charge_dist = 0,
							power_percent = 2.0,
							chain_hit_frame = {},
							particle_data = {},
						},
						{
							"Images/skill_image_3.png",
							"chain",
							"chain_attack",

							"Melee",
							"Chain",

							charge_dist = 0,
							power_percent = 0.5,
							chain_hit_frame = {6, 8, 18},
							particle_data =
							{
								{"snow_pt", "Images/RainParticle.png", 10, 400, 550,
									1.0, 300, 0, -600},
							},
						},
					},
		}

player1 = {info = {"player1", 30, 5, 400, 300, 70, 10, 5000, 4.0, false, true, true, false},

			main_image = {"Characters/akiha.png"},

			image_frame = {},

			ani_image_frame = {
								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"stand",

									{ "aki00_000.png", 1 },
									{ "aki00_001.png", 2 },
									{ "aki00_002.png", 3 },
									{ "aki00_003.png", 4 },
									{ "aki00_004.png", 5 },
									{ "aki00_005.png", 6 },
									{ "aki00_006.png", 7 },
									{ "aki00_007.png", 8 },
									{ "aki00_008.png", 9 },
									{ "aki00_009.png", 10 },
									{ "aki00_010.png", 11 },
									{ "aki00_011.png", 12 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"move",

									{ "aki26_000.png", 1 },
									{ "aki26_001.png", 2 },
									{ "aki26_002.png", 3 },
									{ "aki26_003.png", 4 },
									{ "aki26_004.png", 5 },
									{ "aki26_005.png", 6 },
									{ "aki26_006.png", 7 },
									{ "aki26_007.png", 8 },
									{ "aki26_008.png", 9 },
									{ "aki26_009.png", 10 },
									{ "aki26_010.png", 11 },
									{ "aki26_011.png", 12 },
									{ "aki26_012.png", 13 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"attack",

									{ "aki40_000.png", 1 },
									{ "aki40_001.png", 2 },
									{ "aki40_002.png", 3 },
									{ "aki40_003.png", 4 },
									{ "aki40_004.png", 5 },
									{ "aki40_005.png", 6 },
									{ "aki40_006.png", 7 },
									{ "aki40_007.png", 8 },
									{ "aki40_008.png", 9 },
									{ "aki40_009.png", 10 },
									{ "aki40_010.png", 11 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"dead",

									{ "aki24_008.png", 1 },
									{ "aki24_009.png", 2 },
									{ "aki24_010.png", 3 },
									{ "aki24_011.png", 4 },
									{ "aki24_012.png", 5 },
									{ "aki24_013.png", 6 },
									{ "aki24_014.png", 7 },
									{ "aki24_015.png", 8 },
									{ "aki24_016.png", 9 },
									{ "aki24_017.png", 10 },
									{ "aki24_018.png", 11 },
									{ "aki24_019.png", 12 },
									{ "aki24_020.png", 13 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"akiha_charge",

									{ "aki13_000.png", 1 },
									{ "aki13_001.png", 2 },
									{ "aki13_002.png", 3 },
									{ "aki13_003.png", 4 },
									{ "aki13_004.png", 5 },
									{ "aki13_005.png", 6 },
									{ "aki13_006.png", 7 },
									{ "aki13_007.png", 8 },
									{ "aki13_008.png", 9 },
									{ "aki13_009.png", 10 },
									{ "aki13_010.png", 11 },
									{ "aki13_011.png", 12 },
									{ "aki13_012.png", 13 },
									{ "aki13_013.png", 14 },
									{ "aki13_014.png", 15 },
									{ "aki13_015.png", 16 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"akiha_chain_1",

									{ "aki70_000.png", 1 },
									{ "aki70_001.png", 2 },
									{ "aki70_002.png", 3 },
									{ "aki70_003.png", 4 },
									{ "aki70_004.png", 5 },
									{ "aki70_005.png", 6 },
									{ "aki70_006.png", 7 },
									{ "aki70_007.png", 8 },
									{ "aki70_008.png", 9 },
									{ "aki70_009.png", 10 },
									{ "aki70_010.png", 11 },
									{ "aki70_011.png", 12 },
									{ "aki70_012.png", 13 },
									{ "aki70_013.png", 14 },
									{ "aki70_014.png", 15 },
									{ "aki70_015.png", 16 },
								},

								{
									"Characters/akiha.png",
									"Characters/akiha.plist",
									"akiha_chain_2",

									{ "aki60_000.png", 1 },
									{ "aki60_001.png", 2 },
									{ "aki60_002.png", 3 },
									{ "aki60_003.png", 4 },
									{ "aki60_004.png", 5 },
									{ "aki60_005.png", 6 },
									{ "aki60_006.png", 7 },
									{ "aki60_007.png", 8 },
									{ "aki60_008.png", 9 },
									{ "aki60_009.png", 10 },
									{ "aki60_010.png", 11 },
									{ "aki60_011.png", 12 },
									{ "aki60_012.png", 13 },
									{ "aki60_013.png", 14 },
									{ "aki60_014.png", 15 },
									{ "aki60_015.png", 16 },
									{ "aki60_016.png", 17 },
									{ "aki60_017.png", 18 },
									{ "aki60_018.png", 19 },
									{ "aki60_019.png", 20 },
									{ "aki60_020.png", 21 },
									{ "aki60_021.png", 22 },
									{ "aki60_022.png", 23 },
									{ "aki60_023.png", 24 },
									{ "aki60_024.png", 25 },
									{ "aki60_025.png", 26 },
									{ "aki60_026.png", 27 },
									{ "aki60_027.png", 28 },
									{ "aki60_028.png", 29 },
									{ "aki60_029.png", 30 },
									{ "aki60_030.png", 31 },
									{ "aki60_031.png", 32 },
									{ "aki60_032.png", 33 },
									{ "aki60_033.png", 34 },
									{ "aki60_034.png", 35 },
									{ "aki60_035.png", 36 },
									{ "aki60_036.png", 37 },
									{ "aki60_037.png", 38 },
									{ "aki60_038.png", 39 },
									{ "aki60_039.png", 40 },
									{ "aki60_040.png", 41 },
									{ "aki60_041.png", 42 },
									{ "aki60_042.png", 43 },
									{ "aki60_043.png", 44 },
									{ "aki60_044.png", 45 },
									{ "aki60_045.png", 46 },
								}
							  },

			ani_frame_set = {
								{"stand", 0.02, 0, 0, false},
								{"move", 0.02, 4, 11, false},
								{"attack", 0.04, 0, 0, false},
								{"dead", 0.02, 0, 0, true},
								{"akiha_charge", 0.02, 5, 9, false},
								{"akiha_chain_1", 0.03, 0, 0, false},
								{"akiha_chain_2", 0.03, 0, 0, false},
							},
			effect_set = {
							{
								"Characters/akiha_effect.plist",
								"akiha_chain_2",

								{"aki60_151.png", 0, 0, 5},
								{"aki60_152.png", 0, 0, 6},
								{"aki60_153.png", 0, 0, 7},
								{"aki60_154.png", 0, 0, 8},
								{"aki60_155.png", 0, 0, 9},
								{"aki60_156.png", 0, 0, 10},
								{"aki60_157.png", 0, 0, 11},
								{"aki60_158.png", 0, 0, 12},
								{"aki60_159.png", 0, 0, 13},
								{"aki60_160.png", 0, 0, 14},
								{"aki60_161.png", 0, 0, 15},
								{"aki60_162.png", 0, 0, 16},
								{"aki60_163.png", 0, 0, 17},
								{"aki60_164.png", 0, 0, 18},
								{"aki60_165.png", 0, 0, 19},
								{"aki60_166.png", 0, 0, 20},
								{"aki60_167.png", 0, 0, 21},
								{"aki60_168.png", 0, 0, 22},
								{"aki60_200.png", 100, 0, 25},
								{"aki60_201.png", 100, 0, 26},
								{"aki60_202.png", 100, 0, 27},
								{"aki60_203.png", 100, 0, 28},
								{"aki60_204.png", 100, 0, 29},
								{"aki60_205.png", 100, 0, 30},
								{"aki60_206.png", 100, 0, 31},
								{"aki60_207.png", 100, 0, 32},
								{"aki60_208.png", 100, 0, 33},
								{"aki60_209.png", 100, 0, 34},
								{"aki60_210.png", 100, 0, 35},
								{"aki60_211.png", 100, 0, 36},
								{"aki60_212.png", 100, 0, 37},
								{"aki60_237.png", 100, 0, 38},
								{"aki60_238.png", 100, 0, 39},
								{"aki60_239.png", 100, 0, 40},
								{"aki60_240.png", 100, 0, 41},
								{"aki60_241.png", 100, 0, 42},
								{"aki60_242.png", 100, 0, 43},
								{"aki60_243.png", 100, 0, 44},
								{"aki60_244.png", 100, 0, 45},
								{"aki60_245.png", 100, 0, 46},
							},
						 },
			skill = {
						{
							"Images/skill_image_1.png",
							"akiha_charge",
							"charge_straight",

							"Melee",
							"Charge",

							charge_dist = 500,
							power_percent = 1.2,
							chain_hit_frame = {},
							particle_data = {},
						},
						{
							"Images/skill_image_2.png",
							"akiha_chain_1",
							"power_attack",

							"Melee",
							"Chain",

							charge_dist = 0,
							power_percent = 0.3,
							chain_hit_frame = {5, 6, 7, 8, 9, 10, 11},
							particle_data = {},
						},
						{
							"Images/skill_image_3.png",
							"akiha_chain_2",
							"chain_attack",

							"Melee",
							"Chain",

							charge_dist = 0,
							power_percent = 0.2,
							chain_hit_frame = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
							particle_data = {},
						},
					},
		}

enemy = {info = {"enemy", 15, 5, 200, 400, 400, 100, 2500, 2.0, true, false, false, true},

			main_image = {"Characters/sion.png"},

			image_frame = {},

			ani_image_frame = {
								{
									"Characters/sion.png",
									"Characters/sion.plist",
									"stand",

									{ "sio0000.png", 1 },
									{ "sio0001.png", 2 },
									{ "sio0002.png", 3 },
									{ "sio0003.png", 4 },
									{ "sio0004.png", 5 },
									{ "sio0005.png", 6 },
									{ "sio0006.png", 7 },
									{ "sio0007.png", 8 },
									{ "sio0008.png", 9 },
									{ "sio0009.png", 10 },
									{ "sio0010.png", 11 },
									{ "sio0011.png", 12 },
									{ "sio0012.png", 13 },
									{ "sio0013.png", 14 },
									{ "sio0014.png", 15 },
									{ "sio0015.png", 16 },
									{ "sio0016.png", 17 },
									{ "sio0017.png", 18 },
									{ "sio0018.png", 19 },
									{ "sio0019.png", 20 },
								},

								{
									"Characters/sion.png",
									"Characters/sion.plist",
									"move",

									{ "sio2600.png", 1 },
									{ "sio2601.png", 2 },
									{ "sio2602.png", 3 },
									{ "sio2603.png", 4 },
									{ "sio2604.png", 5 },
									{ "sio2605.png", 6 },
									{ "sio2606.png", 7 },
									{ "sio2607.png", 8 },
									{ "sio2608.png", 9 },
									{ "sio2609.png", 10 },
									{ "sio2610.png", 11 },
								},

								{
									"Characters/sion.png",
									"Characters/sion.plist",
									"attack",

									{ "sio4200.png", 1 },
									{ "sio4201.png", 2 },
									{ "sio4202.png", 3 },
									{ "sio4203.png", 4 },
									{ "sio4204.png", 5 },
									{ "sio4205.png", 6 },
									{ "sio4206.png", 7 },
									{ "sio4207.png", 8 },
									{ "sio4208.png", 9 },
									{ "sio4210.png", 10 },
									{ "sio4211.png", 11 },
									{ "sio4212.png", 12 },
									{ "sio4213.png", 13 },
									{ "sio4214.png", 14 },
									{ "sio4215.png", 15 },
									{ "sio4216.png", 16 },
								},

								{
									"Characters/sion.png",
									"Characters/sion.plist",
									"dead",

									{ "sio2410.png", 1 },
									{ "sio2411.png", 2 },
									{ "sio2412.png", 3 },
									{ "sio2413.png", 4 },
									{ "sio2414.png", 5 },
									{ "sio2415.png", 6 },
									{ "sio2416.png", 7 },
									{ "sio2417.png", 8 },
									{ "sio2418.png", 9 },
									{ "sio2419.png", 10 },
								}
							  },
			ani_frame_set = {
							  {"stand", 0.02, 0, 0, false},
							  {"move", 0.02, 0, 0, false},
							  {"attack", 0.04, 0, 0, false},
							  {"dead", 0.02, 0, 0, true},
							},
			effect_set = {
						 },

			skill = {},
		}

enemy1 = {info = {"enemy1", 10, 5, 200, 300, 100, 10, 2500, 3.0, true, false, false, false},

			main_image = {"Characters/kohaku.png"},

			image_frame = {},

			ani_image_frame = {
								{
									"Characters/kohaku.png",
									"Characters/kohaku.plist",
									"stand",

									{ "koh00_000.png", 1 },
									{ "koh00_001.png", 2 },
									{ "koh00_002.png", 3 },
									{ "koh00_003.png", 4 },
									{ "koh00_004.png", 5 },
									{ "koh00_005.png", 6 },
									{ "koh00_006.png", 7 },
									{ "koh00_007.png", 8 },
								},

								{
									"Characters/kohaku.png",
									"Characters/kohaku.plist",
									"move",

									{ "koh26_000.png", 1 },
									{ "koh26_001.png", 2 },
									{ "koh26_002.png", 3 },
									{ "koh26_003.png", 4 },
									{ "koh26_004.png", 5 },
									{ "koh26_005.png", 6 },
									{ "koh26_006.png", 7 },
									{ "koh26_007.png", 8 },
									{ "koh26_008.png", 9 },
									{ "koh26_009.png", 10 },
									{ "koh26_010.png", 11 },
									{ "koh26_011.png", 12 },
									{ "koh26_012.png", 13 },
									{ "koh26_013.png", 14 },
									{ "koh26_014.png", 15 },
									{ "koh26_015.png", 16 },
									{ "koh26_016.png", 17 },
									{ "koh26_017.png", 18 },
								},

								{
									"Characters/kohaku.png",
									"Characters/kohaku.plist",
									"attack",

									{ "koh03_000.png", 1 },
									{ "koh03_001.png", 2 },
									{ "koh03_002.png", 3 },
									{ "koh03_003.png", 4 },
									{ "koh03_004.png", 5 },
									{ "koh03_005.png", 6 },
									{ "koh03_006.png", 7 },
									{ "koh03_007.png", 8 },
									{ "koh03_008.png", 9 },
									{ "koh03_009.png", 10 },
									{ "koh03_010.png", 11 },
									{ "koh03_011.png", 12 },
									{ "koh03_012.png", 13 },
									{ "koh03_013.png", 14 },
									{ "koh03_014.png", 15 },
									{ "koh03_015.png", 16 },
									{ "koh03_016.png", 17 },
									{ "koh03_017.png", 18 },
									{ "koh03_018.png", 19 },
									{ "koh03_019.png", 20 },
								},

								{
									"Characters/kohaku.png",
									"Characters/kohaku.plist",
									"dead",

									{ "koh24_009.png", 1 },
									{ "koh24_010.png", 2 },
									{ "koh24_011.png", 3 },
									{ "koh24_012.png", 4 },
									{ "koh24_013.png", 5 },
									{ "koh24_014.png", 6 },
									{ "koh24_015.png", 7 },
									{ "koh24_016.png", 8 },
								}
							  },
			ani_frame_set = {
							  {"stand", 0.02, 0, 0, false},
							  {"move", 0.02, 4, 11, false},
							  {"attack", 0.04, 0, 0, false},
							  {"dead", 0.02, 0, 0, true},
							},
			effect_set = {
						 },

			skill = {},
		}

local GameObject = {}

function GameObject.getVarString()
	return {"player", "player1", "enemy", "enemy1"}
end

return GameObject
