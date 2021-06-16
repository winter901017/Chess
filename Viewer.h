/*���*/
#pragma once
#ifndef VIEWER_H
#define VIEWER_H

#include<windows.h>
#include<winusb.h>
#include <cwchar>
#include <math.h>
#include <ctime>
#include <conio.h>  /*  _getch() */

/* �]�m��r�C�� */
/* 119 �L�Ǧ� lightGray */
/* 136 �`�Ǧ� darkGray */
/* 255 �զ� white */
/*   7 �¦� black */
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
	// �ѽL�w�q:
	//  �@�ӴѽL�O 8x8, ��X�@�Ӧ���O 7x13
	//  �]���ѽL�ݭn 64�Ӧ�� �ҥH�w�q�ѽL���}�C�O 7*8=56 13*8=104
	//  board [56][104] �̭��s�񪺬O��r�C��
	//  �̷ӭק��r�C��ӱ���}�C���������C��
	*/

	/* �s��ѽL�C�� */
	int board[56][104] = {};
	position mouseCoordinate;
	/* �^��0 ����Ҧ� */
	/* �^��1 AI��M */
	/* ESC�O���X */
	int menu() {
		/* �]�m�r��j�p ��³��w */
		consoleTextSize(26);
		cout << "**************************************" << endl;
		cout << endl;
		cout << "Author�GB10930234 B10730218 B10730001" << endl;
		cout << "Project 2 : Chess ��v��" << endl;
		cout << endl;
		cout << "**************************************" << endl;
		cout << "        �п�J����ӿ�ܼҦ�\n" << endl;
		cout << "        Menu: ����Ҧ� : 0" << endl;
		cout << "                AI�Ҧ� : 1\n" << endl;
		cout << "             ESC�����}" << endl;
		while (1) {
			char c = _getch();
			if (c == '0' || c == '1') return 0;
			if (c == 27) exit(0);

		};
	}

	/* �]�m��r�C�� */
	/* 119 �L�Ǧ� */
	/* 136 �`�Ǧ� */
	/* 255 �զ� */
	/*   7 �¦� */
	void setColor(int color = BLACK) {
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	/* �N8x8���y���ഫ���������y�� */
	position coordinate_To_Pixel(int y, int x) {
		position coordinate;
		coordinate.y = y * 7;
		coordinate.x = x * 13;
		return coordinate;
	}

	/* �N8x8���y���ഫ���������y�� */
	position pixel_To_Coordinate(position pixel) {
		position coordinate;
		coordinate.y = pixel.y / 7;
		coordinate.x = pixel.x / 13;
		return coordinate;
	}

	/* ��l�ƴѽL */
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

	/* ��X�ѽL */
	void printBoard() {
		ios_base::sync_with_stdio(false); /* �[�tcout */
		cin.tie(NULL); /* �[�tcout */

		for (int y = 0; y < 56; y++) {
			for (int x = 0; x < 104; x++) {
				/* �ӵۨC�Ӥ@��Pixel color��X */
				this->setColor(board[y][x]);
				cout << ' ';
			}
			this->setColor(BLACK); /* ��X����]��O�@�ؿ�X�r�� �ҥH�n�]���¦��קKbug */
			cout << '\n';
		}
		/* �]�w�^�¦� */
		this->setColor(BLACK);
	}

	/*  �]�m�ѽL  �i�H�����ϥ�canmove�ѼƩ�J */
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

		/* ���ഫ�������y�� */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);

		/* �n�Ϥ��C�� ���M�ܩ� */
		int color;
		if (board[coordinate.y][coordinate.x] == DARKGRAY)color = 102; //�`��
		else color = 238; //�L��

		/* �~��ذ_�� */
		for (int y = 0; y < 7; y++) {
			for (int x = 0; x < 13; x++) {
				if (x == 0 || x == 12 || x == 1 || x == 11 || y == 0 || y == 6) {
					board[coordinate.y + y][coordinate.x + x] = color;
				}
			}
		}
	};

	/*  �]�m�ѽL  �i�H�����ϥ�board�ѼƩ�J */
	void setBoard(Board& const board) {
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				position coordinate = { y,x };
				chessMan(board.position[y][x], coordinate);
			}
		}
	}

	/*  ��ܴ����P�C�� */
	void chessMan(int weights, position coordinate) {
		int color = 0;
		if (weights > 0) color = WHITE;
		else if (weights < 0) color = BLACK;

		switch (abs(weights)) {
			/* ��l */
		case 0:
			lattice(coordinate);
			break;
			/* ���� */
		case 1: case 2:
			rook(coordinate, color);
			break;
			/* �M�h */
		case 3: case 4:
			knight(coordinate, color);
			break;
			/* �D�� */
		case 5: case 6:
			bishop(coordinate, color);
			break;
			/* �ӦZ */
		case 7:
			queen(coordinate, color);
			break;
			/* ��� */
		case 8:
			king(coordinate, color);
			break;
			/* �h�L */
		case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:
			parwn(coordinate, color);
			break;
		}
	}

	/* ��l */
	void lattice(position coordinate) {
		/* ���ഫ�������y�� */
		coordinate = coordinate_To_Pixel(coordinate.y, coordinate.x);
		for (int y = 0; y < 7; y++)	{
			for (int x = 0; x < 13; x++) {
				board[coordinate.y + y][coordinate.x + x] = board[coordinate.y][coordinate.x];
			}
		}
	}
	/* �h�L */
	void parwn(position coordinate, int color)
	{
		/* ���ഫ�������y�� */
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
	/* ���� */
	void rook(position coordinate, int color)
	{
		/* ���ഫ�������y�� */
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
	/* �M�h */
	void knight(position coordinate, int color)
	{
		/* ���ഫ�������y�� */
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
	/* �D�� */
	void bishop(position coordinate, int color)
	{
		/* ���ഫ�������y�� */
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
	/* �ӦZ */
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
	/* ��� */
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

	/* �M���e�� �ѨM�ϥ� system("cls") �|���{�{���D */
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

	/*  Console�]�����ù� */
	void consoleFullScreen()
	{
		HWND hwnd = GetForegroundWindow();
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

	/*  Console�]�m��r�j�p */
	void consoleTextSize(int size)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = size;                  // �ק��r�j�p
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		//std::wcscpy(cfi.FaceName, L"Console"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

	/*  �ƹ����� */
	/* �������I������ �h���y�Ц^��true */
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
				/* �ѽL�~�y�Ф���Ĳ�o */
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
		/* �N�ƹ��������y���ഫ���ѽL */
		mouseCoordinate = pixel_To_Coordinate(mouseCoordinate);
	}

};

#endif // !VIEWER_H