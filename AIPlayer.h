/*白*/
#pragma once
#include"Player.h"

class AIPlayer :public Player {
public:

	AIPlayer(bool c) : Player(true) {}//建構
	~AIPlayer() {};//解構

	virtual void KingCanMove(Board& const board, int y, int x, bool c) override;//國王移動
	virtual void QueenCanMove(Board& const board, int y, int x, bool c)override;//皇后移動
	virtual void BishopCanMove(Board& const board, int y, int x, bool c)override;//主教移動
	virtual void KnightCanMove(Board& const board, int y, int x, bool c)override;//騎士移動
	virtual void RookCanMove(Board& const board, int y, int x, bool c)override;//城堡移動
	virtual void ParwnCanMove(Board& const board, int y, int x, bool c)override;//士兵移動

	virtual int OnMove(Board& const board, int y, int x)override; //選擇要移動的棋子
	virtual void Moving(Board& const board, int chess, int to_i, int to_j)override;//移動到的位置
	virtual void OnPromote(Board& const board, int chess, int to_y, int to_x)override; //選擇升階類型
};

void AIPlayer::KingCanMove(Board& const board, int y, int x, bool c) {
	//判斷是否可移動
	//九宮格
	if (y - 1 > 0 && x - 1 > 0)	canMove[y - 1][x - 1] = 1;
	if (y - 1 > 0) canMove[y - 1][x] = 1;
	if (y - 1 > 0 && x + 1 < 8)	canMove[y - 1][x + 1] = 1;
	if (x - 1 > 0) canMove[y][x - 1] = 1;
	if (x + 1 < 8) canMove[y][x + 1] = 1;
	if (y + 1 < 8 && x - 1 > 0)	canMove[y + 1][x - 1] = 1;
	if (y + 1 < 8) canMove[y + 1][x] = 1;
	if (y + 1 < 8 && x + 1 < 8)	canMove[y + 1][x + 1] = 1;

	//路上是否有棋子擋住
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (canMove[i][j] == 1) {
				if (board.position[i][j] > 0) canMove[i][j] = 0;//我方擋住								
				if (board.position[i][j] < 0) canMove[i][j] = 2;//敵方擋住
			}
		}
	}
}
void AIPlayer::QueenCanMove(Board& const board, int y, int x, bool c) {
	//判斷是否可移動
	bool isBlock = false;//被阻擋

	for (int i = 0; i < 8; i++) {
		if (y - i >= 0 && x - i >= 0)canMove[y - i][x - i] = true;//左上			
		if (y - i >= 0 && x + i < 8)canMove[y - i][x + i] = true;//右上			
		if (y + i < 8 && x - i >= 0)canMove[y + i][x - i] = true;//左下			
		if (y + i < 8 && x + i < 8)canMove[y + i][x + i] = true;//右下			
		canMove[i][x] = true;/*直*/canMove[y][i] = true;/*橫*/
	}

	//路上是否有棋子擋住
		//這段跟主教一樣
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x - i] < 0) {
			canMove[y - i][x - i] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x + i] < 0) {
			canMove[y - i][x - i] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x - i] < 0) {
			canMove[y - i][x - i] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x + i] < 0) {
			canMove[y + i][x + i] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y + i][x + i] != 2) canMove[y + i][x + i] = 0;
	}
	isBlock = false;

	//這段跟城堡一樣
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x] < 0) {
			canMove[y - i][x] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y - i][x] != 2) canMove[y - i][x] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x] < 0) {
			canMove[y + i][x] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y + i][x] != 2) canMove[y + i][x] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y][x - i] < 0) {
			canMove[y + i][x] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y + i][x] != 2) canMove[y][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y][x + i] < 0) {
			canMove[y + i][x] = 2;
			isBlock = true;
		}
		if (isBlock && canMove[y + i][x] != 2) canMove[y][x + i] = 0;
	}
	//原本的位置
	canMove[y][x] = false;


}
void AIPlayer::BishopCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//被阻擋
			//判斷是否可移動
	for (int i = 0; i < 8; i++) {
		if (y - i >= 0 && x - i >= 0) canMove[y - i][x - i] = true;//左上
		if (y - i >= 0 && x + i < 8) canMove[y - i][x + i] = true;//右上
		if (y + i < 8 && x - i >= 0) canMove[y + i][x - i] = true;//左下
		if (y + i < 8 && x + i < 8) canMove[y + i][x + i] = true;//右下
	}
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x - i] != 0) isBlock = true;
		if (isBlock) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x + i] != 0) isBlock = true;
		if (isBlock) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x - i] != 0) isBlock = true;
		if (isBlock) canMove[y - i][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x + i] != 0) isBlock = true;
		if (isBlock) canMove[y + i][x + i] = 0;
	}
	isBlock = false;

	//原本的位置
	canMove[y][x] = 0;

}
void AIPlayer::KnightCanMove(Board& const board, int y, int x, bool c) {
	//判斷是否可移動
	if (y - 2 >= 0 && x - 1 >= 0) canMove[y - 2][x - 1] = true;//上左		
	if (y - 2 >= 0 && x + 1 < 8) canMove[y - 2][x + 1] = true;//上右
	if (y + 2 < 8 && x - 1 >= 0) canMove[y + 2][x - 1] = true;//下左
	if (y + 2 < 8 && x + 1 < 8) canMove[y + 2][x + 1] = true;//下右
	if (y - 1 >= 0 && x + 2 < 8) canMove[y - 1][x + 2] = true;//右上
	if (y + 1 < 8 && x + 2 < 8) canMove[y + 1][x + 2] = true;//右下
	if (y - 1 >= 0 && x - 2 >= 0) canMove[y - 1][x - 2] = true;//左上
	if (y + 1 < 8 && x - 2 >= 0) canMove[y + 1][x - 2] = true;//左下

	//路上是否有棋子擋住
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (canMove[i][j] == 1) {
				if (board.position[i][j] > 0) canMove[i][j] = 0;//我方擋住								
				if (board.position[i][j] < 0) canMove[i][j] = 2;//敵方擋住
			}
		}
	}

}
void AIPlayer::RookCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//被阻擋
	//判斷是否可移動
	for (int i = 0; i < 8; i++) {
		canMove[i][x] = true;//直
		canMove[y][i] = true;//橫
	}
	//路上是否有棋子擋住
	for (int i = 1; i < 8; i++) {
		if (board.position[y - i][x] != 0) isBlock = true;
		if (isBlock)canMove[y - i][x] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y + i][x] != 0)isBlock = true;
		if (isBlock)canMove[y + i][x] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y][x - i] != 0)isBlock = true;
		if (isBlock)canMove[y][x - i] = 0;
	}
	isBlock = false;
	for (int i = 1; i < 8; i++) {
		if (board.position[y][x + i] > 0)isBlock = true;
		if (isBlock)canMove[y][x + i] = 0;
	}
	//原本的位置
	canMove[y][x] = false;

}
void AIPlayer::ParwnCanMove(Board& const board, int y, int x, bool c) {
	int chess = board.position[y][x];
	//判斷是否可移動
	if (isFirstStep[chess - 9] && board.position[y - 2][x] == 0) canMove[y - 2][x] = 1;//第一步
	if (board.position[y - 1][x] == 0) canMove[y - 1][x] = 1;//無阻擋		

	//斜吃
	if (y - 1 > 0 && x + 1 < 8 && board.position[y - 1][x + 1] < 0)
		canMove[y - 1][x + 1] = 2;
	if (y - 1 > 0 && x - 1 > 0 && board.position[y - 1][x - 1] < 0)
		canMove[y - 1][x - 1] = 2;
	//第一步
	isFirstStep[chess - 9] = false;
}

int AIPlayer::OnMove(Board& const board, int y, int x) {
	int chess = board.position[y][x];
	bool color = false;

	switch (chess) {
	case 8://國王
		KingCanMove(board, y, x, color);
		break;
	case 7://皇后
		QueenCanMove(board, y, x, color);
		break;
	case 6: case 5://主教
		BishopCanMove(board, y, x, color);
		break;
	case 4: case 3://騎士
		KnightCanMove(board, y, x, color);
		break;
	case 2: case 1://城堡
		RookCanMove(board, y, x, color);
		break;
	case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16://士兵
		ParwnCanMove(board, y, x, color);
		break;
	default://錯誤的棋子
		cout << "提示：輪到另一方\n";
		canmove = false;
		break;
	}

	//是否可移動
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (canMove[i][j] != 0) canmove = true;
		}
	}
	if (canmove == false) cout << "無法移動棋子，請重新輸入\n";

	//改回來
	if (color == true) chess *= -1;
	return chess;
}

void AIPlayer::Moving(Board& const board, int chess, int to_y, int to_x) {
	//不可移動
	if (canMove[to_y][to_x] == 0) {
		cout << "錯誤的座標，請重新輸入\n";
		caneat = false;
	}
	else caneat = true;

	if (caneat) {
		//原本位置歸零
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board.position[i][j] == chess) board.position[i][j] = 0;
			}
		}

		//改盤面
		board.position[to_y][to_x] = chess;

		//士兵升階
		if (chess >= 9 && chess <= 16 && to_y == 0) OnPromote(board, chess, to_y, to_x);//白

		//清空canMove
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				canMove[i][j] = 0;
			}
		}
	}
}

void AIPlayer::OnPromote(Board& const board, int chess, int to_y, int to_x) {
	int chosenchess;
	cout << "將將將~~恭喜~~\n";
	cout << "選擇要升級成什麼棋子\n";
	cin >> chosenchess;
	while (chosenchess == 0 || chosenchess > 8 || chosenchess < -8) {
		if (chosenchess == 0) cout << "請選擇棋子\n";
		else cout << "無法升級成該棋子，請重新輸入\n";
		cin >> chosenchess;
	}
	switch (chosenchess) {
	case 8: case -8:
		board.position[to_y][to_x] = 8;
		cout << "變成國王\n";
		break;
	case 7: case -7:
		board.position[to_y][to_x] = 7;
		cout << "變成皇后\n";
		break;
	case 6: case 5: case -6: case -5:
		board.position[to_y][to_x] = 6;
		cout << "變成主教\n";
		break;
	case 4: case 3: case -4: case -3:
		board.position[to_y][to_x] = 4;
		cout << "變成騎士\n";
		break;
	case 2: case 1: case -2: case -1:
		if (chess > 0) board.position[to_y][to_x] = 2;
		cout << "變成城堡\n";
		break;
	}
}