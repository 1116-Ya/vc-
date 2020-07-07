#include "stdafx.h"
#include "Start.h"
#include <curses.h>
#include "Setting.h"

Change_Check CP;

//スタート画面
void Start() {

	int  x, y, w, h;
	
	initscr();

	getmaxyx(stdscr, h, w);
	y = h / 2;
	x = (w - 20) / 2;
	int key;
	int s=1000;
	int Mode = 0;
	keypad(stdscr, TRUE);
	
	//モード選択と設定画面への遷移
	while (1)
	{
		erase();
		mvprintw(y - 8 + (s % 4) * 2, x - 2, "⇒");
		mvprintw(y - 10, x, "Othello Game");
		mvprintw(y - 8, x, "二人で対戦");
		mvprintw(y - 6, x, "敵と対戦");
		mvprintw(y - 4, x, "シミュレーション");
		mvprintw(y - 2, x, "設定");
		refresh();
		
		key = getch();

		if (key == 'a') {
			if (s % 4 == 0) {
				Mode = 0;
				break;
				
			}
			else if (s % 4 == 1) {
				if (readInt("Player1", "first", 4, "Settei.ini") == 1) {
					Mode = 1;
				}
				else {
					Mode = 2;
				}
				break;
			}
			else if (s % 4 == 2) {
				Mode = 3;
				break;
			}
			else if (s % 4 == 3) {

				Option();
			}
		}
		switch (key) {
		case KEY_UP:	s--; break;
		case KEY_DOWN:	s++; break;
		}
		


	}
	//モードをiniファイルに書き込む
	writeInt("Mode", "mode", Mode, "Settei.ini");
	
}

//設定画面
void Option() {

	int  x, y, w, h;
	int key;
	int s = 1000;
	CP.x = 0;
	CP.y = 0;
	CP.z = 0;
	
	getmaxyx(stdscr, h, w);
	y = h / 2;
	x = (w - 20) / 2;
	keypad(stdscr, TRUE);

	int change = 0;
	char name[40];
	char name2[40];
	int i = 1000;
	
	//プレイヤーネームの変更とコンピュータと対戦する際の先行か後攻の確認
	while (1)
	{
		erase();
		mvprintw(y - 10+(s % 4) * 2, x-2, "⇒");
		mvprintw(y - 10, x, "プレイヤー1名前変更");
		if (change == 1) {
			move(y - 10, x + 22);
			getstr(name);
			
		}
		mvprintw(y - 8, x, "プレイヤー2名前変更");
		if (change == 2) {
			erase();
			mvprintw(y - 8, x, "プレイヤー2名前変更");
			refresh();
			move(y - 8, x + 22);
			getstr(name2);

		}
		mvprintw(y - 6, x, "コンピュータ対戦での先行後攻");
		if (change == 3) {
			while (1)
			{

				
				erase();
				mvprintw(y - 6, x, "コンピュータ対戦での先行後攻");
				mvprintw(y - 8, x + 32 + ((i % 2) * 8), "▼");
				mvprintw(y - 6, x + 30, "First    Second");
				refresh();
				key = getch();
				if (key == 'a') {
					break;
				}

				switch (key) {
				case KEY_RIGHT:	i++; break;
				case KEY_LEFT:	i--; break;
				}
			}

		}
		mvprintw(y - 4, x, "設定終了");
		refresh();

		if (change == 0) {
			key = getch();

			if (key == 'a') {
				if (s % 4 == 0) {
					change = 1;
					CP.x = 1;
				}
				else if (s % 4 == 1) {
					change = 2;
					CP.y = 1;
				}
				else if (s % 4== 2) {
					change = 3;
					CP.z = 1;
				}
				else if (s % 4 == 3) {
					erase();
					break;
				}
				
			}
			switch (key) {
			case KEY_UP:	s--; break;
			case KEY_DOWN:	s++; break;
			}
		}
		else
		{
			change = 0;
		}
		

	}

	//変更があった場合にiniファイルに書き込む
	if (CP.x == 1) {
		writeChar("Player1", "name", name, "Settei.ini");
	}
	if (CP.y == 1) {
		writeChar("Player2", "name", name2, "Settei.ini");
	}if (CP.z == 1) {
		writeInt("Player1", "first", (i%2)+1, "Settei.ini");
	}
	

}

