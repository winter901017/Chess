/*顯示*/
#pragma once
#ifndef VIEWER_H
#define VIEWER_H

#include<windows.h>
#include<winusb.h>
#include <cwchar>
#include <math.h>
#include <ctime>
#include <conio.h>  /*  _getch() */

/* 設置文字顏色 */
/* 119 淺灰色 lightGray */
/* 136 深灰色 darkGray */
/* 255 白色 white */
/*   7 黑色 black */
const int LIGHTGRAY = 119;
const int DARKGRAY = 136;
const int WHITE = 255;
const int BLACK = 7;

struct position {
	int y;
	int x;
};

class Viewer {
public:
	/*
	// 棋盤定義:
	//  一個棋盤是 8x8, 輸出一個色塊是 7x13
	//  因為棋盤需要 64個色塊 所以定義棋盤的陣列是 7*8=56 13*8=104
	//  board [56][104] 裡面存放的是文字顏色
	//  依照修改文字顏色來控制陣列中像素的顏色
	*/

	/* 存放棋盤顏色 */
	int board[56][104] = {};
	position mouseCoordinate;
	/* 回傳0 單機模式 */
	/* 回傳1 AI對決 */
	/* ESC是跳出 */
	int menu() {
		/* 設置字體大小 單純喜歡 */
		consoleTextSize(26);
		cout << "**************************************" << endl;
		cout << endl;
		cout << "Author：B10930234 B10730218 B10730001" << endl;
		cout << "Project 2 : Chess 西洋棋" << endl;
		cout << endl;
		cout << "**************************************" << endl;
		cout << "        請輸入按鍵來選擇模式\n" << endl;
		cout << "        Menu: 單機模式 : 0" << endl;
		cout << "                AI模式 : 1\n" << endl;
		cout << "             ESC鍵離開" << endl;
		while (1) {
			char c = _getch();
			if (c == '0' || c == '1') return 0;
			if (c == 27) exit(0);

		};
	}

	/* 設置文字顏色 */
	/* 119 淺灰色 */
	/* 136 深灰色 */
	/* 255 白色 */
	/*   7 黑色 */
	void setColor(int color = BLACK) {
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	/* 將8x8的座標轉換成像素的座標 */
	position coordinate_To_Pixel(int y, int x) {
		position coordinate;
		coordinate.y = y * 7;
		coordinate.x = x * 13;
		return coordinate;
	}

	/* 將8x8的座標轉換成像素的座標 */
	position pixel_To_Coordinate(position pixel) {
		position coordinate;
		coordinate.y = pixel.y / 7;
		coordinate.x = pixel.x / 13;
		return coordinate;
	}

	/* 初始化棋盤 */
	void initializeBoard() {
		int col = 0;
		for (int y = 0; y < 56; y++) {
			int row = 0;
			for (int x = 0; x < 104; x++) {
				if (x > 0 && (x % 13 == 0))row++;
				if (row % 2 == 0) {
					if (col % 2 == 0) board[y][x] = DARKGRAY;
					else board[y][x] = LIGHTGRAY;
				}
				else {
					if (col % 2 == 0) board[y][x] = LIGHTGRAY;
					else board[y][x] = DARKGRAY;
				};
			}
			if (((y + 1) % 7) == 0 && y != 0) col++;
		}
	}

	/* 輸出棋盤 */
	void printBoard() {
		ios_base::sync_with_stdio(false); /* 加速cout */
		cin.tie(NULL); /* 加速cout */

		for (int y = 0; y < 56; y++) {
			for (int x = 0; x < 104; x++) {
				/* 照著每個一的Pixel color輸出 */
				this->setColor(board[y][x]);
				cout << ' ';
			}
			this->setColor(BLACK); /* 輸出換行也算是一種輸出字元 所以要設成黑色避免bug */
			cout << '\n';
		}
		/* 設定回黑色 */
		this->setColor(BLACK);
	}

	/*  設置棋盤  可以直接使用canmove參數放入 */
	void setCanMove(const int board[8][8]) {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				position coordinate = { y,x };
				canMove(board[y][x], coordinate);
			}
		}
	}

	void canMove(int weights, position coordinate) {
		if (weights == 0) return;

		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		/* 要區分顏色 不然很怪 */
		int color;
		if (board[coordinate.y][coordinate.x] == DARKGRAY)color = 102; //深色
		else color = 238; //淺色

		/* 外圍框起來 */
		for (int y = 0; y < 7; y++) {
			for (int x = 0; x < 13; x++) {
				if (x == 0 || x == 12 || x == 1 || x == 11 || y == 0 || y == 6) {
					board[coordinate.y + y][coordinate.x + x] = color;
				}
			}
		}
	};

	/*  設置棋盤  可以直接使用board參數放入 */
	void setBoard(Board& const board) {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				position coordinate = { y,x };
				chessMan(board.position[y][x], coordinate);
			}
		}
	}

	/*  選擇棋類與顏色 */
	void chessMan(int weights, position coordinate) {
		int color = 0;
		if (weights > 0) color = WHITE;
		else if (weights < 0) color = BLACK;

		switch (abs(weights)) {
			/* 格子 */
		case 0:
			lattice(coordinate);
			break;
			/* 城堡 */
		case 1: case 2:
			rook(coordinate, color);
			break;
			/* 騎士 */
		case 3: case 4:
			knight(coordinate, color);
			break;
			/* 主教 */
		case 5: case 6:
			bishop(coordinate, color);
			break;
			/* 皇后 */
		case 7:
			queen(coordinate, color);
			break;
			/* 國王 */
		case 8:
			king(coordinate, color);
			break;
			/* 士兵 */
		case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:
			parwn(coordinate, color);
			break;
		}
	}

	/* 格子 */
	void lattice(position coordinate) {
		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);
		for (int y = 0; y < 7; y++)	{
			for (int x = 0; x < 13; x++) {
				board[coordinate.y + y][coordinate.x + x] = board[coordinate.y][coordinate.x];
			}
		}
	}
	/* 士兵 */
	void parwn(position coordinate, int color)
	{
		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);
		board[coordinate.y + 2][coordinate.x + 5] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 7] = color;

		board[coordinate.y + 3][coordinate.x + 4] = color;
		board[coordinate.y + 3][coordinate.x + 5] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 7] = color;
		board[coordinate.y + 3][coordinate.x + 8] = color;

		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;
	}
	/* 城堡 */
	void rook(position coordinate, int color)
	{
		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		board[coordinate.y + 1][coordinate.x + 4] = color;
		board[coordinate.y + 1][coordinate.x + 6] = color;
		board[coordinate.y + 1][coordinate.x + 8] = color;

		board[coordinate.y + 2][coordinate.x + 4] = color;
		board[coordinate.y + 2][coordinate.x + 5] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 7] = color;
		board[coordinate.y + 2][coordinate.x + 8] = color;

		board[coordinate.y + 3][coordinate.x + 5] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 7] = color;


		board[coordinate.y + 4][coordinate.x + 4] = color;
		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;
		board[coordinate.y + 4][coordinate.x + 8] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;
	}
	/* 騎士 */
	void knight(position coordinate, int color)
	{
		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		board[coordinate.y + 1][coordinate.x + 4] = color;
		board[coordinate.y + 1][coordinate.x + 5] = color;
		board[coordinate.y + 1][coordinate.x + 6] = color;
		board[coordinate.y + 1][coordinate.x + 7] = color;
		board[coordinate.y + 1][coordinate.x + 8] = color;

		board[coordinate.y + 2][coordinate.x + 3] = color;
		board[coordinate.y + 2][coordinate.x + 4] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 7] = color;
		board[coordinate.y + 2][coordinate.x + 8] = color;
		board[coordinate.y + 2][coordinate.x + 9] = color;

		board[coordinate.y + 3][coordinate.x + 5] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 7] = color;
		board[coordinate.y + 3][coordinate.x + 8] = color;
		board[coordinate.y + 3][coordinate.x + 9] = color;

		board[coordinate.y + 4][coordinate.x + 4] = color;
		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;
		board[coordinate.y + 4][coordinate.x + 8] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;
	}
	/* 主教 */
	void bishop(position coordinate, int color)
	{
		/* 先轉換成像素座標 */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		board[coordinate.y + 1][coordinate.x + 6] = color;


		board[coordinate.y + 2][coordinate.x + 5] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 7] = color;


		board[coordinate.y + 3][coordinate.x + 4] = color;
		board[coordinate.y + 3][coordinate.x + 5] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 7] = color;
		board[coordinate.y + 3][coordinate.x + 9] = color;


		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;
		board[coordinate.y + 4][coordinate.x + 8] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;
	}
	/* 皇后 */
	void queen(position coordinate, int color)
	{
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		board[coordinate.y + 1][coordinate.x + 3] = color;
		board[coordinate.y + 1][coordinate.x + 5] = color;
		board[coordinate.y + 1][coordinate.x + 7] = color;
		board[coordinate.y + 1][coordinate.x + 9] = color;

		board[coordinate.y + 2][coordinate.x + 4] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 8] = color;



		board[coordinate.y + 3][coordinate.x + 5] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 7] = color;


		board[coordinate.y + 4][coordinate.x + 4] = color;
		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;
		board[coordinate.y + 4][coordinate.x + 8] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;
	}
	/* 國王 */
	void king(position coordinate, int color)
	{
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		board[coordinate.y + 1][coordinate.x + 4] = color;
		board[coordinate.y + 1][coordinate.x + 6] = color;
		board[coordinate.y + 1][coordinate.x + 8] = color;

		board[coordinate.y + 2][coordinate.x + 3] = color;
		board[coordinate.y + 2][coordinate.x + 5] = color;
		board[coordinate.y + 2][coordinate.x + 6] = color;
		board[coordinate.y + 2][coordinate.x + 7] = color;
		board[coordinate.y + 2][coordinate.x + 9] = color;


		board[coordinate.y + 3][coordinate.x + 4] = color;
		board[coordinate.y + 3][coordinate.x + 6] = color;
		board[coordinate.y + 3][coordinate.x + 8] = color;

		board[coordinate.y + 4][coordinate.x + 4] = color;
		board[coordinate.y + 4][coordinate.x + 5] = color;
		board[coordinate.y + 4][coordinate.x + 6] = color;
		board[coordinate.y + 4][coordinate.x + 7] = color;
		board[coordinate.y + 4][coordinate.x + 8] = color;

		board[coordinate.y + 5][coordinate.x + 3] = color;
		board[coordinate.y + 5][coordinate.x + 4] = color;
		board[coordinate.y + 5][coordinate.x + 5] = color;
		board[coordinate.y + 5][coordinate.x + 6] = color;
		board[coordinate.y + 5][coordinate.x + 7] = color;
		board[coordinate.y + 5][coordinate.x + 8] = color;
		board[coordinate.y + 5][coordinate.x + 9] = color;

	}

	/* 清除畫面 解決使用 system("cls") 會有閃爍問題 */
	void clr()
	{
		system("cls");
		/*
		COORD cursorPosition;
		cursorPosition.X = 0;
		cursorPosition.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
		*/
	}

	/*  Console設為全螢幕 */
	void consoleFullScreen()
	{
		HWND hwnd = GetForegroundWindow();
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

	/*  Console設置文字大小 */
	void consoleTextSize(int size)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = size;                  // 修改文字大小
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		//std::wcscpy(cfi.FaceName, L"Console"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

	/*  滑鼠偵測 */
	/* 當偵測到點擊左鍵 則更改座標回傳true */
	bool mouseDetect()
	{
		position coordinate;

		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD InputRecord;
		DWORD Events;
		COORD coord;
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(hout, &cci);
		SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

		ReadConsoleInput(hin, &InputRecord, 1, &Events);

		if (InputRecord.EventType == MOUSE_EVENT)
		{
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				//coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
				//coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				//SetConsoleCursorPosition(hout, coord);
				//SetConsoleTextAttribute(hout, rand() % 7 + 9);
				mouseCoordinate.x = InputRecord.Event.MouseEvent.dwMousePosition.X;
				mouseCoordinate.y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				//cout << mouseCoordinate.y << " " << mouseCoordinate.x<<endl;
				/* 棋盤外座標不做觸發 */
				if ((mouseCoordinate.x > 103) || (mouseCoordinate.y > 55))
				{
					return false;
				};
				mouseCoordinate = pixel_To_Coordinate(mouseCoordinate);
				//cout << mouseCoordinate.y << " " << mouseCoordinate.x << endl;
				return true;
			}
		}
		FlushConsoleInputBuffer(hin);

		return false;
	}

	void mouseEvent(){
		/* 將滑鼠的像素座標轉換成棋盤 */
		mouseCoordinate = pixel_To_Coordinate(mouseCoordinate);
	}

};

#endif // !VIEWER_H