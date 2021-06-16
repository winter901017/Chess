/*紀錄棋子的位置、狀態*/
/*	正(白):0 負(黑):1
	0:無棋子
	1 2:城堡 前後左右不限步數
	3 4:騎士 L(無視九宮格內是否有棋子)
	5 6:主教 斜45度不限步數
	7:皇后 車+象
	8:國王 九宮格
	9~16:士兵 第一步可前進1或2格 之後一次前進一格 只能斜吃 到底可升階成皇后、主教、騎士、城堡(改變數字)
	特殊的移動方法：Castling, Passant Capturing
*/
#pragma once
#include"Player.h"
#include"Board.h"
#include"Viewer.h"
#include<iostream>
using namespace std;

class Player {
public:
	bool isFirstStep[8];//士兵是否為第一步 false否 true是
	int canMove[8][8];//是否可移動 每輪更新(0不可動 1可動 2可吃)	
	bool canmove;//紀錄棋子可否移動
	bool caneat;//紀錄可否移動到選擇的位置
	void print();//印出狀態

	Player(bool c);//建構
	~Player() {};//解構

	virtual void KingCanMove(Board& const board, int y, int x, bool c);//國王移動
	virtual void QueenCanMove(Board& const board, int y, int x, bool c);//皇后移動
	virtual void BishopCanMove(Board& const board, int y, int x, bool c);//主教移動
	virtual void KnightCanMove(Board& const board, int y, int x, bool c);//騎士移動
	virtual void RookCanMove(Board& const board, int y, int x, bool c);//城堡移動
	virtual void ParwnCanMove(Board& const board, int y, int x, bool c);//士兵移動

	virtual int OnMove(Board& const board, int y, int x); //選擇要移動的棋子
	virtual void Moving(Board& const board, int chess, int to_i, int to_j);//移動到的位置
	virtual void OnPromote(Board& const board, int chess, int to_y, int to_x); //選擇升階類型
};

Player::Player(bool c) :canmove(false), caneat(false), isFirstStep() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			canMove[i][j] = false;
		}
		isFirstStep[i] = true;
	}
}

void Player::KingCanMove(Board& const board, int y, int x, bool c) {
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
	if (c == true) {//黑
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] < 0) canMove[i][j] = 0;//我方擋住								
					if (board.position[i][j] > 0) canMove[i][j] = 2;//敵方擋住
				}
			}
		}
	}
	else {//白
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] > 0) canMove[i][j] = 0;//我方擋住								
					if (board.position[i][j] < 0) canMove[i][j] = 2;//敵方擋住
				}
			}
		}
	}
}
void Player::QueenCanMove(Board& const board, int y, int x, bool c) {
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
	if (c == true) {//黑
		//這段跟主教一樣
		for (int i = 1; i < 8; i++) {
			if (board.position[y - i][x - i] > 0) {
				canMove[y - i][x - i] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y - i][x + i] > 0) {
				canMove[y - i][x - i] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y + i][x - i] > 0) {
				canMove[y - i][x - i] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y - i][x - i] != 2) canMove[y - i][x - i] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y + i][x + i] > 0) {
				canMove[y + i][x + i] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y + i][x + i] != 2) canMove[y + i][x + i] = 0;

		}
		isBlock = false;

		//這段跟城堡一樣
		for (int i = 1; i < 8; i++) {
			if (board.position[y - i][x] > 0) {
				canMove[y - i][x] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y - i][x] != 2) canMove[y - i][x] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y + i][x] > 0) {
				canMove[y + i][x] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y + i][x] != 2) canMove[y + i][x] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y][x - i] > 0) {
				canMove[y + i][x] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y + i][x] != 2) canMove[y][x - i] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y][x + i] > 0) {
				canMove[y + i][x] = 2;
				isBlock = true;
			}
			if (isBlock && canMove[y + i][x] != 2) canMove[y][x + i] = 0;
		}
	}
	else {//白
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
	}
	//原本的位置
	canMove[y][x] = false;


}
void Player::BishopCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//被阻擋
			//判斷是否可移動
	for (int i = 0; i < 8; i++) {
		if (y - i >= 0 && x - i >= 0) canMove[y - i][x - i] = true;//左上
		if (y - i >= 0 && x + i < 8) canMove[y - i][x + i] = true;//右上
		if (y + i < 8 && x - i >= 0) canMove[y + i][x - i] = true;//左下
		if (y + i < 8 && x + i < 8) canMove[y + i][x + i] = true;//右下
	}
	if (c == true) {//黑
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
	}
	else {//白
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
	}

	//原本的位置
	canMove[y][x] = 0;

}
void Player::KnightCanMove(Board& const board, int y, int x, bool c) {
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
	if (c == true) {//黑
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] < 0) canMove[i][j] = 0;//我方擋住								
					if (board.position[i][j] > 0) canMove[i][j] = 2;//敵方擋住
				}
			}
		}
	}
	else {//白
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] > 0) canMove[i][j] = 0;//我方擋住								
					if (board.position[i][j] < 0) canMove[i][j] = 2;//敵方擋住
				}
			}
		}
	}

}
void Player::RookCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//被阻擋
	//判斷是否可移動
	for (int i = 0; i < 8; i++) {
		canMove[i][x] = true;//直
		canMove[y][i] = true;//橫
	}
	//路上是否有棋子擋住
	if (c == true) {//黑					
		for (int i = 1; i < 8; i++) {
			if (board.position[y - i][x] != 0) isBlock = true;
			if (isBlock)canMove[y - i][x] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y + i][x] != 0) isBlock = true;
			if (isBlock)canMove[y + i][x] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y][x - i] != 0) isBlock = true;
			if (isBlock)canMove[y][x - i] = 0;
		}
		isBlock = false;
		for (int i = 1; i < 8; i++) {
			if (board.position[y][x + i] != 0) isBlock = true;
			if (isBlock)canMove[y][x + i] = 0;
		}
	}
	else {//白
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
	}
	//原本的位置
	canMove[y][x] = false;

}
void Player::ParwnCanMove(Board& const board, int y, int x, bool c) {
	int chess = board.position[y][x];
	//黑色
	if (c == true) {
		chess *= -1;
		//判斷是否可移動
		if (board.position[y + 1][x] == 0)//無阻擋
			canMove[y + 1][x] = 1;
		if (isFirstStep[chess - 9] && board.position[y + 2][x] == 0) //第一步
			canMove[y + 2][x] = 1;

		//斜吃
		if (y + 1 < 8 && x + 1 < 8 && board.position[y + 1][x + 1] > 0)
			canMove[y + 1][x + 1] = 2;
		if (y + 1 < 8 && x - 1 > 0 && board.position[y + 1][x - 1] > 0)
			canMove[y + 1][x - 1] = 2;
	}
	//白色
	else {
		//判斷是否可移動
		if (isFirstStep[chess - 9] && board.position[y - 2][x] == 0) canMove[y - 2][x] = 1;//第一步
		if (board.position[y - 1][x] == 0) canMove[y - 1][x] = 1;//無阻擋		

		//斜吃
		if (y - 1 > 0 && x + 1 < 8 && board.position[y - 1][x + 1] < 0)
			canMove[y - 1][x + 1] = 2;
		if (y - 1 > 0 && x - 1 > 0 && board.position[y - 1][x - 1] < 0)
			canMove[y - 1][x - 1] = 2;
	}
	//第一步
	isFirstStep[chess - 9] = false;
}

int Player::OnMove(Board& const board, int y, int x) {
	int chess = board.position[y][x];
	bool color = false;//預設成白色
	canmove = false;

	if (chess < 0) {//黑色
		chess *= -1;
		color = true;
	}
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

void Player::Moving(Board& const board, int chess, int to_y, int to_x) {
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
		if (chess <= -9 && chess >= -16 && to_y == 7) OnPromote(board, chess, to_y, to_x);//黑
		else if (chess >= 9 && chess <= 16 && to_y == 0) OnPromote(board, chess, to_y, to_x);//白

		//清空canMove
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				canMove[i][j] = 0;
			}
		}
	}
}

void Player::OnPromote(Board& const board, int chess, int to_y, int to_x) {
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
		if (chess > 0) board.position[to_y][to_x] = 8;
		else board.position[to_y][to_x] = -8;
		cout << "變成國王\n";
		break;
	case 7: case -7:
		if (chess > 0) board.position[to_y][to_x] = 7;
		else board.position[to_y][to_x] = -7;
		cout << "變成皇后\n";
		break;
	case 6: case 5: case -6: case -5:
		if (chess > 0) board.position[to_y][to_x] = 6;
		else board.position[to_y][to_x] = -6;
		cout << "變成主教\n";
		break;
	case 4: case 3: case -4: case -3:
		if (chess > 0) board.position[to_y][to_x] = 4;
		else board.position[to_y][to_x] = -4;
		cout << "變成騎士\n";
		break;
	case 2: case 1: case -2: case -1:
		if (chess > 0) board.position[to_y][to_x] = 2;
		else board.position[to_y][to_x] = -2;
		cout << "變成城堡\n";
		break;
	}
}

void Player::print() {
	cout << "is First Step: ";
	for (int i = 0; i < 8; i++) {
		cout << isFirstStep[i] << ' ';
	}
	cout << endl;
}