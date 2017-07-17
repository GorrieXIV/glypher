//includes
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define WIDTH 50
#define HEIGHT 20

//#include <menu.h>

void print_menu(WINDOW *menu_win, int highlight);
void menuSwitch (int ch);
void workSwitch (int ch);

int mail = 0;
int page = 0;
int mode = 0;

char *menu[] = {"contacts", "inbox", "new", "archive"};

int main() {

	WINDOW *main_win;
	WINDOW *menu_win;
	WINDOW *work_win;
	int ch;

	initscr();
		clear();		
		cbreak();
		noecho();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_CYAN, COLOR_BLACK);

	refresh();	

	main_win = newwin(HEIGHT, WIDTH, 0, 0);
	menu_win = newwin(2, WIDTH-2, 2, 1);
	work_win = newwin(HEIGHT-5, WIDTH-2, 4, 1);

	keypad(menu_win, TRUE);

	box(main_win, 0, 0);
	wborder(menu_win, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
	box(work_win, 0, 0);

	mvwprintw(main_win, 1, 2, "glypher 2017");
	print_menu(menu_win, page);

	wrefresh(main_win);
	wrefresh(menu_win);
	wrefresh(work_win);
	
	
	while (ch != 'q') {
		ch = wgetch(menu_win);
		if (!mode) {
			menuSwitch(ch);
		} else {
			workSwitch(ch);
		}

		print_menu(menu_win, page);
		wrefresh(menu_win);
		wrefresh(work_win);
	}

	delwin(main_win);
	delwin(menu_win);
	delwin(work_win);
	endwin();
	return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
	int x = 1;
	int y = 1;

	mvwprintw(menu_win, y, x, "|");
	x++;
	int i;
	for (i = 0; i <= 3; i++) {
		if (i == 1) {
			mvwprintw(menu_win, y, x, " ");
			x++;
			if (i == highlight) {wattron(menu_win, A_REVERSE);}
			mvwprintw(menu_win, y, x, "inbox (%d)", mail);
			if (i == highlight) {wattroff(menu_win, A_REVERSE);}
			x = x + 10;
			mvwprintw(menu_win, y, x, " |");
			x = x + 2;
		} else {
			mvwprintw(menu_win, y, x, " ");
			x++;
			if (i == highlight) {wattron(menu_win, A_REVERSE);}
			mvwprintw(menu_win, y, x, menu[i]);
			if (i == highlight) {wattroff(menu_win, A_REVERSE);}
			x = x + strlen(menu[i]) + 1;
			mvwprintw(menu_win, y, x, " |");
			x = x + 2;
		}
	}
}

void menuSwitch (int ch) {
	switch(ch) {
		case KEY_LEFT:
			//mvwprintw(work_win, 1, 2, "left pressed");
			page = (page==0) ? 3 : (page - 1) % 4;
			break;
		case KEY_RIGHT:
			//mvwprintw(work_win, 1, 2, "right pressed");
			page = (page + 1) % 4;
			break;
		case KEY_DOWN:
			mode = 1;
			break;
	}
}

void workSwitch (int ch) {
	switch(ch) {
		case KEY_UP:
			mode = 0;
			break;
	}
}
