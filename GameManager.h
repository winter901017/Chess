#pragma once
#include"Player.h"
#include"Board.h"
#include"Viewer.h"

class GameManager {
public:
	Player p1, p2;//���a
	Board board;//�L�����A
	Viewer viewer;//���
	int movement;//�����C�����A(���ʡB��ܪ��a�B)
	GameManager();
	~GameManager() {};
};

GameManager::GameManager() :movement(0), p1(true), p2(false) {}
