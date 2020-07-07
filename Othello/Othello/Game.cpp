#include "stdafx.h"
#include "Game.h"
#include <curses.h>
#include "Setting.h"

int Player1 = 1;
int Player2 = 2;

int turn;
int flag = 0;
int Enemy = 0;
int mode;
int turnX;
int skip;

char name1[16];
char name2[16];

//オセロ
void othello() {


	noecho();
	cbreak();
	start_color();
	resize_term(45, 190);

	turnX = 0;
	skip = 0;

	//色の設定
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_GREEN);
	init_pair(4, COLOR_GREEN, COLOR_GREEN);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);


	bkgd(COLOR_PAIR(4));

	//モードを読み込み対戦モードを確認する
	mode = readInt("Mode", "mode", 4, "Settei.ini");
	if (mode != 2) {
		turn = Player1;
	}
	else
	{
		turn = Player2;
	}

	int i, j;
	int data[8][8];
	int key;
	int x = 3;
	int y = 3;
	int put = 0;
	int finish = 0;

	//プレイヤーの設定した名前の確認
	readChar("Player1", "name", "", name1, "Settei.ini");
	readChar("Player2", "name", "", name2, "Settei.ini");

	//初期盤面
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			data[i][j] = 0;
		}
	}
	if (mode != 2) {
		data[3][3] = Player2;
		data[4][3] = Player1;
		data[4][4] = Player2;
		data[3][4] = Player1;
	}
	else
	{
		data[3][3] = Player1;
		data[4][3] = Player2;
		data[4][4] = Player1;
		data[3][4] = Player2;
	}

	erase();
	board(data);
	put = Choice(data, x, y, put);
	refresh();
	napms(500);



	while (1)
	{
		//プレイヤーが配置可能確認し、配置可能であればキー入力待ちにする
		if (mode == 0 || mode == 1 || mode == 2) {
			putCheck(data);
		}
		

		//プレイヤーの操作
		if (skip == 0) {
			if (mode == 0 || (mode == 1 && turn == Player1) || (mode == 2 && turn == Player1)) {
				key = getch();

				if (key == 'q') break;
				switch (key) {
				case KEY_UP:
					if (y > 0) {
						y--;
					}
					break;
				case KEY_DOWN:
					if (y < 7) {
						y++;
					}
					break;
				case KEY_LEFT:
					if (x > 0) {
						x--;
					}
					break;
				case KEY_RIGHT:
					if (x < 7) {
						x++;
					}
					break;
				case 'a':
					put = 1;
					break;
				}
			}

			if (mode == 0 || (mode == 1 && turn == Player1) || (mode == 2 && turn == Player1)) {
				erase();

				put = Choice(data, x, y, put);

				board(data);

				refresh();
				napms(100);
			}
			finish = 0;
		}
		else
		{
			if (turn == Player1) {
				turn = Player2;
			}
			else
			{
				turn = Player1;
			}
			finish++;
			skip = 0;
		}

		//コンピュータの操作
		if ((mode == 1 && turn == Player2) || (mode == 2 && turn == Player2) || mode == 3) {

			erase();

			Enemy_Choice(data);

			board(data);

			refresh();
			napms(100);
			if (skip == 1) {
				finish++;
				skip = 0;
			}
			else
			{
				finish = 0;
			}
		}


		//二度スキップするか、60手すべて指し終えると終了
		if (finish > 2) {
			break;
		}
		if (turnX == 60) {
			break;
		}
	}


	erase();
	Finish(data);


}

//結果表示と結果をcsvファイルに書き込む
void Finish(int data[8][8]) {

	bkgd(COLOR_PAIR(5));

	int h, w;
	getmaxyx(stdscr, h, w);
	h = h / 2;
	w = w / 2;
	attrset(COLOR_PAIR(6));

	//両方の駒の数を確認
	int x, y;
	int Point1 = 0;
	int Point2 = 0;
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (data[x][y] == Player1) {
				Point1++;
			}
			else if (data[x][y] == Player2) {
				Point2++;
			}
		}
	}

	mvprintw(h - 3, w - 20, "Player1  vs  Player2");
	mvprintw(h - 1, w - 20, "  %d         %d  ", Point1, Point2);

	if (Point1 > Point2) {
		if ((mode == 1) || (mode == 2) || (mode == 0)) {
			mvprintw(h + 1, w - 17, "%sの勝ち", name1);
		}
	}
	else if (Point1<Point2)
	{
		if ((mode == 1) || (mode == 2)) {
			mvprintw(h + 1, w - 17, "Computerの勝ち");
		}
		else if (mode == 0)
		{
			mvprintw(h + 1, w - 17, "%sの勝ち", name2);
		}

	}
	else
	{
		if ((mode == 1) || (mode == 2) || (mode == 0)) {
			mvprintw(h + 1, w - 17, "引き分け");
		}
	}
	mvprintw(h + 5, w - 17, "Press any key");
	int key;
	key = getch();

	//ファイルに書き込んでいる
	FILE *fp;
	errno_t error;
	error = fopen_s(&fp, "Kekka.csv", "a");

	char s[1024];
	char color1[100];
	char color2[100];
	if (mode != 2) {
		sprintf_s(color1, 100, "BLACK");
		sprintf_s(color2, 100, "WHITE");
	}
	else
	{
		sprintf_s(color1, 100, "WHITE");
		sprintf_s(color2, 100, "BLACK");
	}
	sprintf_s(s, 1024, "\n%d,%s,%s,%d,%s,%s,%d,%d", mode, name1, color1, Point1, name2, color2, Point2, turnX);
	if (error != 0) {

	}
	else {


		fputs(s, fp);

		fclose(fp);
	}


}

//プレイヤー用のスキップ確認
void putCheck(int data[8][8]) {
	int x, y, i, j;
	int Check = 0;
	int Count = 0;
	//順番のプレイヤーが配置可能か確認する
	if (turn == Player1) {
		for (y = 0; y < 8; y++) {
			for (x = 0; x < 8; x++) {
				if (data[x][y] == 0) {
					for (j = 0; j < 3; j++) {
						for (i = 0; i < 3; i++) {
							if (i == 1 && j == 1) {
								continue;
							}

							if ((x + i - 1 < 0) || (x + i - 1 > 7) || (y + j - 1 < 0) || (y + j - 1 > 7)) {
								continue;
							}

							if (data[x + i - 1][y + j - 1] == Player2) {
								Count++;
								while (1)
								{

									if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player2) {
										Count++;
									}
									else if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player1) {
										if (Count > 1) {
											Check++;
										}
										break;
									}
									else {
										break;
									}
								}
							}
							Count = 0;
							if (Check == 1) {
								break;
							}

						}
						if (Check == 1) {
							break;
						}
					}
					if (Check == 1) {
						break;
					}
				}
				if (Check == 1) {
					break;
				}
			}
			if (Check == 1) {
				break;
			}
		}
	}
	else {
		for (y = 0; y < 8; y++) {
			for (x = 0; x < 8; x++) {
				if (data[x][y] == 0) {
					for (j = 0; j < 3; j++) {
						for (i = 0; i < 3; i++) {
							if (i == 1 && j == 1) {
								continue;
							}
							if ((x + i - 1 < 0) || (x + i - 1 > 7) || (y + j - 1 < 0) || (y + j - 1 > 7)) {
								continue;
							}
							if (data[x + i - 1][y + j - 1] == Player1) {
								Count++;
								while (1)
								{

									if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player1) {
										Count++;
									}
									else if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player2) {
										if (Count > 1) {
											Check++;
										}
										break;
									}
									else {
										break;
									}
								}
							}
							Count = 0;
							if (Check == 1) {
								break;
							}

						}
						if (Check == 1) {
							break;
						}
					}
					if (Check == 1) {
						break;
					}
				}
				if (Check == 1) {
					break;
				}
			}
			if (Check == 1) {
				break;
			}
		}
	}
	//配置不可だったときスキップする
	if (Check == 0) {
		skip++;
	}
}

//敵用Choice関数
void Enemy_Choice(int data[8][8]) {

	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;
	int m = 0;
	int n = 0;

	int Value = 0;
	int Point = -100000;

	int grid[2] = { 0,0 };

	//変更可能な予測用盤面データ
	int data2[8][8];
	for (n = 0; n < 8; n++) {
		for (m = 0; m < 8; m++) {
			data2[m][n] = data[m][n];
		}
	}
	//全マス目に対して配置可能か確認する
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (data[x][y] == Player2 || data[x][y] == Player1) {
				continue;
			}
			//座標の周りを配置可能か確認する
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == 1 && j == 1) {
						continue;
					}

					Enemy_Check(data2, x, y, j - 1, i - 1);

				}
			}
			//配置可能だった場合、配置後の盤面を送り盤面評価して一番評価値の高い座標を保持する
			if (Enemy == 1) {
				Value = valuation(data2);
				if (Value > Point) {
					Point = Value;
					grid[0] = x;
					grid[1] = y;
				}
				for (n = 0; n < 8; n++) {
					for (m = 0; m < 8; m++) {
						data2[m][n] = data[m][n];
					}
				}
				Enemy = 0;
			}
		}
	}

	Enemy = 0;
	if (flag == 1) {
		//一度でも置ける場所があった場合
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (i == 1 && j == 1) {
					continue;
				}

				Check(data, grid[0], grid[1], j - 1, i - 1);

			}
		}
	}
	else if (flag == 0) {
		//配置可能でなかったときスキップして相手の順番にする
		if (turn == Player1) {
			turn = Player2;
			skip++;
		}
		else
		{
			turn = Player1;
			skip++;
		}
	}

	if (flag == 1 && turn == Player1) {
		turn = Player2;
		turnX++;
		flag = 0;
	}
	else if (flag == 1 && turn == Player2) {
		turn = Player1;
		turnX++;
		flag = 0;
	}

}

//敵用Check関数
void Enemy_Check(int data[8][8], int x, int y, int i, int j) {
	int change = 0;
	int Count = 0;

	if (turn == Player1) {
		while (1)
		{
			if (x + j + (Count*j) < 0 || x + j + (Count*j) > 7 || y + i + (Count * i) < 0 || y + i + (Count * i) > 7) {
				break;
			}

			if (data[x + j + (Count*j)][y + i + (Count * i)] == 0) {
				break;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player2) {
				Count++;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player1) {
				if (Count != 0) {
					while (1)
					{
						data[x + j + (Count*j)][y + i + (Count * i)] = Player1;
						Count = Count - 1;
						if (Count < -1)
						{
							break;
						}
					}
					change = 1;
				}

				if (Count <= 0) {
					break;
				}
			}


			if (change == 1) {
				break;
			}

		}
	}
	else
	{
		while (1)
		{
			if (x + j + (Count*j) < 0 || x + j + (Count*j) > 7 || y + i + (Count * i) < 0 || y + i + (Count * i) > 7) {
				break;
			}

			if (data[x + j + (Count*j)][y + i + (Count * i)] == 0) {
				break;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player1) {
				Count++;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player2) {
				if (Count != 0) {
					while (1)
					{
						data[x + j + (Count*j)][y + i + (Count * i)] = Player2;
						Count = Count - 1;
						if (Count < -1)
						{
							break;
						}
					}
					change = 1;
				}
				if (Count <= 0) {
					break;
				}
			}



			if (change == 1) {
				break;
			}

		}
	}

	if (change == 1) {
		flag = 1;
		Enemy = 1;
	}
}

//操作するプレイヤー用の配置確認
int Choice(int data[8][8], int x, int y, int put) {

	int i = 0;
	int j = 0;

	attrset(COLOR_PAIR(1));
	mvprintw(((y * 2) + 12), ((x * 4) + 70), "▼");

	//指定した場所に駒がある場合置いていないこととする
	if (data[x][y] != 0) {
		put = 0;
	}

	//Aボタンが押された場合
	if (put == 1) {

		//指定した場所の周りを見て置けるかを判定
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (i == 1 && j == 1) {
					continue;
				}

				Check(data, x, y, j - 1, i - 1);

			}
		}
		//配置出来た時ターンを相手にして、かかった手数を増やす
		if (flag == 1 && turn == Player1) {
			turn = Player2;
			turnX++;
			flag = 0;
		}
		else if (flag == 1 && turn == Player2) {
			turn = Player1;
			turnX++;
			flag = 0;
		}



	}

	return 0;
}

//配置可能か確認後ひっくり返す
void Check(int data[8][8], int x, int y, int j, int i) {
	int change = 0;
	int Count = 0;
	//方向に沿って進み、領域外と何も配置されていなければ終了し、一つ以上相手の駒が存在した先に、自分の駒が配置されている場合配置する
	if (turn == Player1) {
		while (1)
		{

			if (data[x + j + (Count*j)][y + i + (Count * i)] == 0) {
				break;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player2) {
				Count++;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player1) {
				if (Count != 0) {
					while (1)
					{

						data[x + j + (Count*j)][y + i + (Count * i)] = Player1;
						Count = Count - 1;
						if (Count < -1)
						{
							break;
						}
					}
					change = 1;
				}

				if (Count <= 0) {
					break;
				}
			}

			if (x + j + (Count*j) < 0 || x + j + (Count*j) > 7 || y + i + (Count * i) < 0 || y + i + (Count * i) > 7) {
				break;
			}

			if (change == 1) {
				break;
			}

		}
	}
	else
	{
		while (1)
		{
			if (data[x + j + (Count*j)][y + i + (Count * i)] == 0) {
				break;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player1) {
				Count++;
			}
			else if (data[x + j + (Count*j)][y + i + (Count * i)] == Player2) {
				if (Count != 0) {
					while (1)
					{
						data[x + j + (Count*j)][y + i + (Count * i)] = Player2;
						Count = Count - 1;
						if (Count < -1)
						{
							break;
						}
					}
					change = 1;
				}
				if (Count <= 0) {
					break;
				}
			}

			if (x + j + (Count*j) < 0 || x + j + (Count*j) > 7 || y + i + (Count * i) < 0 || y + i + (Count * i) > 7) {
				break;
			}

			if (change == 1) {
				break;
			}

		}
	}
	//配置出来た場合、合図を出す
	if (change == 1) {
		flag = 1;
	}
}

//座標から盤面評価
int valuation(int data[8][8]) {
	//重み関数
	int weight[8][8] = {
		{ 10000,-2000,80,70,70,80,-2000,10000 },
		{ -2000,-3000,30,30,30,30,-3000,-2000 },
		{ 70,30,30,40,40,30,30,70 },
		{ 20,20,80,300,300,80,20,20 },
		{ 20,20,80,300,300,80,20,20 },
		{ 70,30,30,40,40,30,30,70 },
		{ -2000,-3000,30,30,30,30,-3000,-2000 },
		{ 10000,-2000,80,70,70,80,-2000,10000 } };

	int Black_point = 0;
	int White_point = 0;
	int x, y;

	//黒と白の点から値を返す
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (data[x][y] == Player1) {
				Black_point = Black_point + weight[x][y];
			}
			else if (data[x][y] == Player2) {
				White_point = White_point + weight[x][y];
			}
		}
	}
	//重みと次の相手プレイヤーの配置可能数から計算する
	if (turn == Player1) {
		White_point = White_point + (putValue(data, Player2) * 100);
		return Black_point - White_point;
	}
	else if (turn == Player2) {
		Black_point = Black_point + (putValue(data, Player1) * 100);
		return White_point - Black_point;
	}
}

//配置可能位置数を返す
int putValue(int data[8][8], int Next) {
	int x, y, i, j;
	int Check = 0;
	int Count = 0;

	if (Next == Player1) {
		for (y = 0; y < 8; y++) {
			for (x = 0; x < 8; x++) {
				if (data[x][y] == 0) {
					for (j = 0; j < 3; j++) {
						for (i = 0; i < 3; i++) {
							if (i == 1 && j == 1) {
								continue;
							}

							if ((x + i - 1 < 0) || (x + i - 1 > 7) || (y + j - 1 < 0) || (y + j - 1 > 7)) {
								continue;
							}

							if (data[x + i - 1][y + j - 1] == Player2) {
								Count++;
								while (1)
								{

									if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player2) {
										Count++;
									}
									else if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player1) {
										if (Count > 1) {
											Check++;
										}
										break;
									}
									else {
										break;
									}
								}
							}
							Count = 0;


						}

					}

				}
			}
		}
	}
	else {
		for (y = 0; y < 8; y++) {
			for (x = 0; x < 8; x++) {
				if (data[x][y] == 0) {
					for (j = 0; j < 3; j++) {
						for (i = 0; i < 3; i++) {
							if (i == 1 && j == 1) {
								continue;
							}
							if ((x + i - 1 < 0) || (x + i - 1 > 7) || (y + j - 1 < 0) || (y + j - 1 > 7)) {
								continue;
							}
							if (data[x + i - 1][y + j - 1] == Player1) {
								Count++;
								while (1)
								{

									if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player1) {
										Count++;
									}
									else if (data[x + ((i - 1)*Count)][y + ((j - 1)*Count)] == Player2) {
										if (Count > 1) {
											Check++;
										}
										break;
									}
									else {
										break;
									}
								}
							}
							Count = 0;

						}

					}

				}
			}
		}
	}
	return Check;
}

//現状の盤面を表示
void board(int data[8][8]) {

	int y = 0;
	int x = 0;

	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			if (data[x][y] == 0) {
				attrset(COLOR_PAIR(3));
				mvprintw(((y * 2) + 13), ((x * 4) + 70), "□");
			}
			else if (data[x][y] == Player2) {
				if (mode != 2) {
					attrset(COLOR_PAIR(1));
				}
				else
				{
					attrset(COLOR_PAIR(2));
				}
				mvprintw(((y * 2) + 13), ((x * 4) + 70), "●");
			}
			else if (data[x][y] == Player1)
			{
				if (mode != 2) {
					attrset(COLOR_PAIR(2));
				}
				else
				{
					attrset(COLOR_PAIR(1));
				}
				mvprintw(((y * 2) + 13), ((x * 4) + 70), "●");
			}
		}
	}
	field();
}

void field() {
	attrset(COLOR_PAIR(1));

	//順番のプレイヤーを上に表示
	if (turn == Player2)
	{

		mvprintw(9, 76, "%sの番です", name2);
		if (mode != 2) {
			attrset(COLOR_PAIR(1));
		}
		else
		{
			attrset(COLOR_PAIR(2));
		}
		mvprintw(9, 72, "●");
	}
	else if (turn == Player1)
	{
		mvprintw(9, 76, "%sの番です", name1);
		if (mode != 2) {
			attrset(COLOR_PAIR(2));
		}
		else
		{
			attrset(COLOR_PAIR(1));
		}
		mvprintw(9, 72, "●");
	}
	attrset(COLOR_PAIR(1));

	mvprintw(19, 50, "↑");
	mvprintw(20, 48, "←");
	mvprintw(20, 52, "→");
	mvprintw(21, 50, "↓");
	mvprintw(21, 50, "↓");
	mvprintw(22, 48, "A　決定");
	mvprintw(23, 48, "Q　終了");


	//座標の表示
	mvprintw(11, 70, "１");
	mvprintw(11, 74, "２");
	mvprintw(11, 78, "３");
	mvprintw(11, 82, "４");
	mvprintw(11, 86, "５");
	mvprintw(11, 90, "６");
	mvprintw(11, 94, "７");
	mvprintw(11, 98, "８");

	mvprintw(13, 66, "１");
	mvprintw(15, 66, "２");
	mvprintw(17, 66, "３");
	mvprintw(19, 66, "４");
	mvprintw(21, 66, "５");
	mvprintw(23, 66, "６");
	mvprintw(25, 66, "７");
	mvprintw(27, 66, "８");
}