/*�����Ѥl����m�B���A*/
/*	��(��):0 �t(��):1
	0:�L�Ѥl
	1 2:���� �e�ᥪ�k�����B��
	3 4:�M�h L(�L���E�c�椺�O�_���Ѥl)
	5 6:�D�� ��45�פ����B��
	7:�ӦZ ��+�H
	8:��� �E�c��
	9~16:�h�L �Ĥ@�B�i�e�i1��2�� ����@���e�i�@�� �u��צY �쩳�i�ɶ����ӦZ�B�D�СB�M�h�B����(���ܼƦr)
	�S�����ʤ�k�GCastling, Passant Capturing
*/
#pragma once
#include"Player.h"
#include"Board.h"
#include"Viewer.h"
#include<iostream>
using namespace std;

class Player {
public:
	bool isFirstStep[8];//�h�L�O�_���Ĥ@�B false�_ true�O
	int canMove[8][8];//�O�_�i���� �C����s(0���i�� 1�i�� 2�i�Y)	
	bool canmove;//�����Ѥl�i�_����
	bool caneat;//�����i�_���ʨ��ܪ���m
	void print();//�L�X���A

	Player(bool c);//�غc
	~Player() {};//�Ѻc

	virtual void KingCanMove(Board& const board, int y, int x, bool c);//�������
	virtual void QueenCanMove(Board& const board, int y, int x, bool c);//�ӦZ����
	virtual void BishopCanMove(Board& const board, int y, int x, bool c);//�D�в���
	virtual void KnightCanMove(Board& const board, int y, int x, bool c);//�M�h����
	virtual void RookCanMove(Board& const board, int y, int x, bool c);//��������
	virtual void ParwnCanMove(Board& const board, int y, int x, bool c);//�h�L����

	virtual int OnMove(Board& const board, int y, int x); //��ܭn���ʪ��Ѥl
	virtual void Moving(Board& const board, int chess, int to_i, int to_j);//���ʨ쪺��m
	virtual void OnPromote(Board& const board, int chess, int to_y, int to_x); //��ܤɶ�����
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
	//�P�_�O�_�i����
	//�E�c��
	if (y - 1 > 0 && x - 1 > 0)	canMove[y - 1][x - 1] = 1;
	if (y - 1 > 0) canMove[y - 1][x] = 1;
	if (y - 1 > 0 && x + 1 < 8)	canMove[y - 1][x + 1] = 1;
	if (x - 1 > 0) canMove[y][x - 1] = 1;
	if (x + 1 < 8) canMove[y][x + 1] = 1;
	if (y + 1 < 8 && x - 1 > 0)	canMove[y + 1][x - 1] = 1;
	if (y + 1 < 8) canMove[y + 1][x] = 1;
	if (y + 1 < 8 && x + 1 < 8)	canMove[y + 1][x + 1] = 1;

	//���W�O�_���Ѥl�צ�
	if (c == true) {//��
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] < 0) canMove[i][j] = 0;//�ڤ�צ�								
					if (board.position[i][j] > 0) canMove[i][j] = 2;//�Ĥ�צ�
				}
			}
		}
	}
	else {//��
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] > 0) canMove[i][j] = 0;//�ڤ�צ�								
					if (board.position[i][j] < 0) canMove[i][j] = 2;//�Ĥ�צ�
				}
			}
		}
	}
}
void Player::QueenCanMove(Board& const board, int y, int x, bool c) {
	//�P�_�O�_�i����
	bool isBlock = false;//�Q����

	for (int i = 0; i < 8; i++) {
		if (y - i >= 0 && x - i >= 0)canMove[y - i][x - i] = true;//���W			
		if (y - i >= 0 && x + i < 8)canMove[y - i][x + i] = true;//�k�W			
		if (y + i < 8 && x - i >= 0)canMove[y + i][x - i] = true;//���U			
		if (y + i < 8 && x + i < 8)canMove[y + i][x + i] = true;//�k�U			
		canMove[i][x] = true;/*��*/canMove[y][i] = true;/*��*/
	}

	//���W�O�_���Ѥl�צ�
	if (c == true) {//��
		//�o�q��D�Ф@��
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

		//�o�q�򫰳��@��
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
	else {//��
		//�o�q��D�Ф@��
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

		//�o�q�򫰳��@��
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
	//�쥻����m
	canMove[y][x] = false;


}
void Player::BishopCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//�Q����
			//�P�_�O�_�i����
	for (int i = 0; i < 8; i++) {
		if (y - i >= 0 && x - i >= 0) canMove[y - i][x - i] = true;//���W
		if (y - i >= 0 && x + i < 8) canMove[y - i][x + i] = true;//�k�W
		if (y + i < 8 && x - i >= 0) canMove[y + i][x - i] = true;//���U
		if (y + i < 8 && x + i < 8) canMove[y + i][x + i] = true;//�k�U
	}
	if (c == true) {//��
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
	else {//��
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

	//�쥻����m
	canMove[y][x] = 0;

}
void Player::KnightCanMove(Board& const board, int y, int x, bool c) {
	//�P�_�O�_�i����
	if (y - 2 >= 0 && x - 1 >= 0) canMove[y - 2][x - 1] = true;//�W��		
	if (y - 2 >= 0 && x + 1 < 8) canMove[y - 2][x + 1] = true;//�W�k
	if (y + 2 < 8 && x - 1 >= 0) canMove[y + 2][x - 1] = true;//�U��
	if (y + 2 < 8 && x + 1 < 8) canMove[y + 2][x + 1] = true;//�U�k
	if (y - 1 >= 0 && x + 2 < 8) canMove[y - 1][x + 2] = true;//�k�W
	if (y + 1 < 8 && x + 2 < 8) canMove[y + 1][x + 2] = true;//�k�U
	if (y - 1 >= 0 && x - 2 >= 0) canMove[y - 1][x - 2] = true;//���W
	if (y + 1 < 8 && x - 2 >= 0) canMove[y + 1][x - 2] = true;//���U

	//���W�O�_���Ѥl�צ�
	if (c == true) {//��
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] < 0) canMove[i][j] = 0;//�ڤ�צ�								
					if (board.position[i][j] > 0) canMove[i][j] = 2;//�Ĥ�צ�
				}
			}
		}
	}
	else {//��
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (canMove[i][j] == 1) {
					if (board.position[i][j] > 0) canMove[i][j] = 0;//�ڤ�צ�								
					if (board.position[i][j] < 0) canMove[i][j] = 2;//�Ĥ�צ�
				}
			}
		}
	}

}
void Player::RookCanMove(Board& const board, int y, int x, bool c) {
	bool isBlock = false;//�Q����
	//�P�_�O�_�i����
	for (int i = 0; i < 8; i++) {
		canMove[i][x] = true;//��
		canMove[y][i] = true;//��
	}
	//���W�O�_���Ѥl�צ�
	if (c == true) {//��					
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
	else {//��
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
	//�쥻����m
	canMove[y][x] = false;

}
void Player::ParwnCanMove(Board& const board, int y, int x, bool c) {
	int chess = board.position[y][x];
	//�¦�
	if (c == true) {
		chess *= -1;
		//�P�_�O�_�i����
		if (board.position[y + 1][x] == 0)//�L����
			canMove[y + 1][x] = 1;
		if (isFirstStep[chess - 9] && board.position[y + 2][x] == 0) //�Ĥ@�B
			canMove[y + 2][x] = 1;

		//�צY
		if (y + 1 < 8 && x + 1 < 8 && board.position[y + 1][x + 1] > 0)
			canMove[y + 1][x + 1] = 2;
		if (y + 1 < 8 && x - 1 > 0 && board.position[y + 1][x - 1] > 0)
			canMove[y + 1][x - 1] = 2;
	}
	//�զ�
	else {
		//�P�_�O�_�i����
		if (isFirstStep[chess - 9] && board.position[y - 2][x] == 0) canMove[y - 2][x] = 1;//�Ĥ@�B
		if (board.position[y - 1][x] == 0) canMove[y - 1][x] = 1;//�L����		

		//�צY
		if (y - 1 > 0 && x + 1 < 8 && board.position[y - 1][x + 1] < 0)
			canMove[y - 1][x + 1] = 2;
		if (y - 1 > 0 && x - 1 > 0 && board.position[y - 1][x - 1] < 0)
			canMove[y - 1][x - 1] = 2;
	}
	//�Ĥ@�B
	isFirstStep[chess - 9] = false;
}

int Player::OnMove(Board& const board, int y, int x) {
	int chess = board.position[y][x];
	bool color = false;//�w�]���զ�
	canmove = false;

	if (chess < 0) {//�¦�
		chess *= -1;
		color = true;
	}
	switch (chess) {
	case 8://���
		KingCanMove(board, y, x, color);
		break;
	case 7://�ӦZ
		QueenCanMove(board, y, x, color);
		break;
	case 6: case 5://�D��
		BishopCanMove(board, y, x, color);
		break;
	case 4: case 3://�M�h
		KnightCanMove(board, y, x, color);
		break;
	case 2: case 1://����
		RookCanMove(board, y, x, color);
		break;
	case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16://�h�L
		ParwnCanMove(board, y, x, color);
		break;
	default://���~���Ѥl
		cout << "���ܡG����t�@��\n";
		canmove = false;
		break;
	}

	//�O�_�i����
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (canMove[i][j] != 0) canmove = true;
		}
	}
	if (canmove == false) cout << "�L�k���ʴѤl�A�Э��s��J\n";

	//��^��
	if (color == true) chess *= -1;
	return chess;
}

void Player::Moving(Board& const board, int chess, int to_y, int to_x) {
	//���i����
	if (canMove[to_y][to_x] == 0) {
		cout << "���~���y�СA�Э��s��J\n";
		caneat = false;
	}
	else caneat = true;

	if (caneat) {
		//�쥻��m�k�s
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board.position[i][j] == chess) board.position[i][j] = 0;
			}
		}

		//��L��
		board.position[to_y][to_x] = chess;

		//�h�L�ɶ�
		if (chess <= -9 && chess >= -16 && to_y == 7) OnPromote(board, chess, to_y, to_x);//��
		else if (chess >= 9 && chess <= 16 && to_y == 0) OnPromote(board, chess, to_y, to_x);//��

		//�M��canMove
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				canMove[i][j] = 0;
			}
		}
	}
}

void Player::OnPromote(Board& const board, int chess, int to_y, int to_x) {
	int chosenchess;
	cout << "�N�N�N~~����~~\n";
	cout << "��ܭn�ɯŦ�����Ѥl\n";
	cin >> chosenchess;
	while (chosenchess == 0 || chosenchess > 8 || chosenchess < -8) {
		if (chosenchess == 0) cout << "�п�ܴѤl\n";
		else cout << "�L�k�ɯŦ��ӴѤl�A�Э��s��J\n";
		cin >> chosenchess;
	}
	switch (chosenchess) {
	case 8: case -8:
		if (chess > 0) board.position[to_y][to_x] = 8;
		else board.position[to_y][to_x] = -8;
		cout << "�ܦ����\n";
		break;
	case 7: case -7:
		if (chess > 0) board.position[to_y][to_x] = 7;
		else board.position[to_y][to_x] = -7;
		cout << "�ܦ��ӦZ\n";
		break;
	case 6: case 5: case -6: case -5:
		if (chess > 0) board.position[to_y][to_x] = 6;
		else board.position[to_y][to_x] = -6;
		cout << "�ܦ��D��\n";
		break;
	case 4: case 3: case -4: case -3:
		if (chess > 0) board.position[to_y][to_x] = 4;
		else board.position[to_y][to_x] = -4;
		cout << "�ܦ��M�h\n";
		break;
	case 2: case 1: case -2: case -1:
		if (chess > 0) board.position[to_y][to_x] = 2;
		else board.position[to_y][to_x] = -2;
		cout << "�ܦ�����\n";
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