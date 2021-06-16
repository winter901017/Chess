#pragma once
#include"Player.h"
#include"Board.h"
#include"Viewer.h"

class GameManager {
public:
	Player p1, p2;//玩家
	Board board;//盤面狀態
	Viewer viewer;//顯示
	int movement;//紀錄遊戲狀態(移動、選擇玩家、)
	GameManager();
	~GameManager() {};
};

GameManager::GameManager() :movement(0), p1(true), p2(false) {}
