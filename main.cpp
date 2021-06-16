#include<iostream>
#include"GameManager.h"

int main() {
	GameManager gm;
	bool color = true;//輪到誰 -1黑色(true) 1白色(false)
	bool isEnd = false;//判斷遊戲是否結束

	/* 選單 */
	gm.viewer.menu();
	/* 全螢幕 */
	gm.viewer.consoleFullScreen();
	/* 設置文字大小 */
	gm.viewer.consoleTextSize(16);
	/* 初始化棋盤顏色 */
	gm.viewer.initializeBoard();
	/* 設置棋盤 */
	gm.viewer.setBoard(gm.board);
	/* 清除畫面 */
	gm.viewer.clr();
	//system("cls");
	/* 輸出 */
	gm.viewer.printBoard();

	int chessmoved = 99;
	while (isEnd == false) {
		/* Player A */
		while (gm.p1.canmove == false) {//不可移動
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				if (gm.board.position[y][x] < 0)chessmoved = gm.p1.OnMove(gm.board, y, x);//顏色是否正確
			}
		}
		gm.p1.canmove = false;

		gm.viewer.setBoard(gm.board); //設置棋盤
		gm.viewer.setCanMove(gm.p1.canMove);
		gm.viewer.clr(); //清除畫面
		gm.viewer.printBoard(); //輸出

		while (gm.p1.caneat == false) {//無法移動到該位置
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
		gm.viewer.setBoard(gm.board); //設置棋盤
		gm.viewer.clr(); //清除畫面
		gm.viewer.printBoard(); //輸出

		color = false;

		/* Player B */
		while (gm.p2.canmove == false && isEnd == false) {//不可移動且遊戲未結束
			if (gm.viewer.mouseDetect()) {
				int y = gm.viewer.mouseCoordinate.y;
				int x = gm.viewer.mouseCoordinate.x;
				if (gm.board.position[y][x] > 0)chessmoved = gm.p2.OnMove(gm.board, y, x);//顏色是否正確
			}
		}
		gm.p2.canmove = false;

		gm.viewer.setBoard(gm.board); //設置棋盤
		gm.viewer.setCanMove(gm.p2.canMove);
		gm.viewer.clr(); //清除畫面
		gm.viewer.printBoard(); //輸出

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
		gm.viewer.setBoard(gm.board); //設置棋盤
		gm.viewer.clr(); //清除畫面
		gm.viewer.printBoard(); //輸出

		color = true;
	}
	return 0;
}
