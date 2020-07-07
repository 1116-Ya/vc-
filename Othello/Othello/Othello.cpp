#include "stdafx.h"
#include "Game.h"
#include <curses.h>
#include "Start.h"
int main(void)
{
	int key;
	int finish=0;
	while (1)
	{

	
		Start();
		othello();
		while (1)
		{
			erase();
			mvprintw(18, 70, "終了する  A");
			mvprintw(20, 70, "もう一度開始する  B");
			refresh();
			key = getch();
			if (key == 'a') {
				finish = 1;
				break;
			}
			else if (key == 'b') {
				break;
			}
		}
		if (finish == 1) {
			endwin();
			break;
			
		}
	

	}
	return 0;
}
