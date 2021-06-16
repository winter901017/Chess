#include<iostream>
#include"GameManager.h"

int main() {
	GameManager gm;
	bool color = true;//����� -1�¦�(true) 1�զ�(false)
	bool isEnd = false;//�P�_�C���O�_����

	/* ��� */
	gm.viewer.menu();
	/* ���ù� */
	gm.viewer.consoleFullScreen();
	/* �]�m��r�j�p */
	gm.viewer.consoleTextSize(16);
	/* ��l�ƴѽL�C�� */
	gm.viewer.initializeBoard();
	/* �]�m�ѽL */
	gm.viewer.setBoard(gm.board);
	/* �M���e�� */
	gm.viewer.clr();
	//system("cls");
	/* ��X */
	gm.viewer.printBoard();

	int chessmoved = 99;
	while (isEnd == false) {
		/* Player A */
		while (gm.p1.canmove == false) {//���i����
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				if (gm.board.position[y][x] < 0)chessmoved = gm.p1.OnMove(gm.board, y, x);//�C��O�_���T
			}
		}
		gm.p1.canmove = false;

		gm.viewer.setBoard(gm.board); //�]�m�ѽL
		gm.viewer.setCanMove(gm.p1.canMove);
		gm.viewer.clr(); //�M���e��
		gm.viewer.printBoard(); //��X

		while (gm.p1.caneat == false) {//�L�k���ʨ�Ӧ�m
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				y = gm.viewer.mouseCoordinate.y;
				x = gm.viewer.mouseCoordinate.x;
				gm.p1.Moving(gm.board, chessmoved, y, x);
			}
		}
		gm.p1.caneat = false;

		gm.viewer.initializeBoard();
		gm.viewer.setBoard(gm.board); //�]�m�ѽL
		gm.viewer.clr(); //�M���e��
		gm.viewer.printBoard(); //��X

		color = false;

		/* Player B */
		while (gm.p2.canmove == false && isEnd == false) {//���i���ʥB�C��������
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				if (gm.board.position[y][x] > 0)chessmoved = gm.p2.OnMove(gm.board, y, x);//�C��O�_���T
			}
		}
		gm.p2.canmove = false;

		gm.viewer.setBoard(gm.board); //�]�m�ѽL
		gm.viewer.setCanMove(gm.p2.canMove);
		gm.viewer.clr(); //�M���e��
		gm.viewer.printBoard(); //��X

		while (gm.p2.caneat == false) {
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				y = gm.viewer.mouseCoordinate.y;
				x = gm.viewer.mouseCoordinate.x;
				gm.p2.Moving(gm.board, chessmoved, y, x);
			}
		}
		gm.p2.caneat = false;

		gm.viewer.initializeBoard();
		gm.viewer.setBoard(gm.board); //�]�m�ѽL
		gm.viewer.clr(); //�M���e��
		gm.viewer.printBoard(); //��X

		color = true;
	}
	return 0;
}
