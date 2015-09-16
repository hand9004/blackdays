stage_1_tutorial =
{
				talk_dialog = "dialog_1.png",
------------ 0 ------------		
				{
					restriction = "Game",
					left = "Images/win_arc00_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "BlackDays의 튜토리얼에 온 것을 환영해!\n\n나는 게임 설명을 도와줄 알퀘이드라고 해!",
					font_size = 20,
					font_color = {0, 0, 0},
				},					
------------ 1 ------------						
				{
					restriction = "Game",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "제일 처음에 나온 UI설명은 잘 봤지?\n\n잘 봤으리라 믿고 바로 게임내 조작 설명할게!",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 2 ------------						
				{
					restriction = "No Restrict",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "다음 알려줄 것은 화면 이동이야!\n\n아무 방향으로 손가락을 쓸어서 맵을 움직여볼까?",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 3 ------------						
				{
					restriction = "No Restrict",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "잘하네! 캐릭터를 이동시켜볼까? 배경의 땅 같은 곳들을 클릭해봐!\n\n주의해야할 것은 가는 길 앞에 장애물이 있다면 이동할 수가 없어.\n\n그렇다면 다른 곳을 터치해서 가야겠지?",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 4 ------------						
				{
					restriction = "Game",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "잘했어! 알아둬야할 건, 카메라가 고정 상태이면 캐릭터를 움직이지 않고는\n\n맵을 움직일 수 없어.고정 상태랑 비고정 상태를 잘 활용해야겠지?",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 5 ------------						
				{
					restriction = "No Restrict",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "맵의 오른편을 보면 적군이 있을거야.\n\n적군을 제거해야 다음을 진행할 수 있을것 같아!\n\n적군을 터치해서 공격해보자!",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 6 ------------							
				{
					restriction = "No Restrict",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "적군이 죽었어! 적군이 죽었으니 자유롭게 움직일 수 있겠어.\n\n맵의 오른쪽으로 계속 가보자.",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 7 ------------						
				{
					restriction = "No Restrict",
					left = "Images/win_arc01_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "앗! 트랩에 걸렸어! 트랩에서 빠져나가려면\n\n노란 게이지바가 찰때까지 터치하면되!",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 8 ------------						
				{
					restriction = "Game",
					left = "Images/win_arc00_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "휴...겨우 트랩에서 풀려났네..조금 당황할 했어.",
					font_size = 20,
					font_color = {0, 0, 0},
				},
------------ 9 ------------						
				{
					restriction = "Game",
					left = "Images/win_arc00_0.png",
					right = "Images/win_kohaku00_0.png",
					message = "기본적인 설명은 여기까지!\n조작해보면 훨씬 이해가 빠를거야!\n맵 끝의 표시된 곳까지 이동하면 튜토리얼 끝이야!\n안녕!",
					font_size = 20,
					font_color = {0, 0, 0},
				},
}

local Conversation = {}

function Conversation.getVarString()
	return {"stage_1_tutorial"}
end

return Conversation