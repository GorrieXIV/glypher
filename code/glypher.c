//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#define WIDTH 50
#define HEIGHT 20

void print_menu(WINDOW *menu_win, int highlight);
void print_workSpace(WINDOW *work_win, int highlight);
void print_Toe();
void menuSwitch (int ch);
void workSwitch (int ch);
void loadData();


//file and email io functions
void readFile (char* file, char* dest, int buffsize);
void writeToFile (char* file, char* message, int buffsize);
long fileSize (char* file);
void parseBuffer (WINDOW *work_win, char *messages, int start);
void sendMessage (char *recp, char *message);

//gpg related functions
void initKeyRing ();

int mail = 0;
int page = 0;
int mode = 0;
int workTab = 0;
int highlight = 0;

char *contacts;
char *messages;
char *archived;

char *menu[] = {"Contacts", "Inbox", "New", "Archive"};

char *toBuf;
char *msgBuf;

int main() {
	initKeyRing();
	loadData();

	toBuf = malloc(sizeof(char)*1);
	msgBuf = malloc(sizeof(char)*1);

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
	keypad(work_win, TRUE);

	box(main_win, 0, 0);
	wborder(menu_win, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
	box(work_win, 0, 0);

	mvwprintw(main_win, 1, 2, "glypher 2017");
	print_menu(menu_win, page);
	print_workSpace(work_win, workTab);

	wrefresh(main_win);
	wrefresh(menu_win);
	wrefresh(work_win);
	

	while (true) {
		ch = wgetch(menu_win);
		if (!mode) {
			menuSwitch(ch);
		} else {
			workSwitch(ch);
		}

		//erase and redraw main_win
		box(main_win, 0, 0);
		mvwprintw(main_win, 1, 2, "glypher 2017");		
		wrefresh(main_win);	

		//draw menu_win
		wborder(menu_win, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
		print_menu(menu_win, page);
		wrefresh(menu_win);
		
		//erase and redraw work_win
		werase(work_win);
		box(work_win, 0, 0);
		print_workSpace(work_win, highlight);
		wrefresh(work_win);
		
		//add nav toe
		print_Toe();
		refresh();
	}

	free(contacts);
	free(messages);
	free(archived);
	free(toBuf);
	free(msgBuf);

	delwin(main_win);
	delwin(menu_win);
	delwin(work_win);
	endwin();
	return 0;
}

// WINDOW PRINTING FUNCTIONS ----------------------------------------------------------------------

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
			mvwprintw(menu_win, y, x, "Inbox (%d)", mail);
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

void print_workSpace(WINDOW *work_win, int highlight) {
	switch (page) {
		case 0:
			if (highlight == 0) {wattron(work_win, A_REVERSE);}
			mvwprintw(work_win, 1, 2,"New");
			if (highlight == 0) {wattroff(work_win, A_REVERSE);}
			mvwprintw(work_win, 1, 5," | ");
			if (highlight == 1) {wattron(work_win, A_REVERSE);}
			mvwprintw(work_win, 1, 8,"Delete");
			if (highlight == 1) {wattroff(work_win, A_REVERSE);}
			mvwprintw(work_win, 1, 14," |");
			mvwprintw(work_win, 2, 1,"_____________________________________________");
			parseBuffer(work_win, contacts, 3);
			break;
		case 1:
			parseBuffer(work_win, messages, 1);
			break;
		case 2:	
			mvwprintw(work_win, 1, 1,"To:");
			mvwprintw(work_win, 1, 5,"%s", toBuf);
			mvwprintw(work_win, 2, 1,"_____________________________________________");
			parseBuffer(work_win, msgBuf, 3);
			mvwprintw(work_win, 12, 1,"_____________________________________________");
			mvwprintw(work_win, 13, 39,"| ");
			if (workTab == 2) {wattron(work_win, A_REVERSE);}
			mvwprintw(work_win, 13, 41,"Send");
			if (workTab == 2) {wattroff(work_win, A_REVERSE);}
			break;
		case 3:
			parseBuffer(work_win, archived, 1);
			break;
	}
}

void print_Toe() {
	if (!mode) {
		mvprintw(3, 0,">");
	} else if (!workTab) {
		mvprintw(5, 0,">");
	} else if (workTab == 1) {
		mvprintw(7, 0,">");
	} else {
		mvprintw(17, 0,">");
	}
}

// WINDOW PRINTING FUNCTIONS ----------------------------------------------------------------------

// MENU CONTROL FUNCTIONS -------------------------------------------------------------------------

void menuSwitch (int ch) {
	switch(ch) {
		case KEY_LEFT:
			page = (page==0) ? 3 : (page - 1) % 4;
			workTab = 0;
			break;
		case KEY_RIGHT:
			page = (page + 1) % 4;
			workTab = 0;
			break;
		case KEY_DOWN:
			mode = 1;
			break;
	}
}

void workSwitch (int ch) {
	//char newch;
	if (page == 2) {
		switch(ch) {
			case KEY_DOWN:
				workTab = (workTab == 0) ? 1 : 2;
				break;
			case KEY_UP:
				mode = (workTab > 0) ? 1 : 0;
				workTab = (workTab == 2) ? 1 : 0;
				break;
			default:
				if (!workTab) {
					//newch = getch();
					if (ch == '\n') {
						workTab = 1;
					} else if (ch == '\b'){//127 || ch == 8) {
						toBuf = realloc(toBuf, strlen(toBuf));
					}else {
						toBuf[strlen(toBuf)] = ch;
						toBuf = realloc(toBuf, strlen(toBuf));
					}
				} else if (workTab == 1) {
					if (ch == '\b'){//127 || ch == 8) {
						msgBuf = realloc(msgBuf, strlen(msgBuf)-1);
					} else {
						msgBuf[strlen(msgBuf)] = ch;
						msgBuf = realloc(msgBuf, strlen(msgBuf)+1);
					}
				} else {
					if (ch == '\n') {
						sendMessage(toBuf, msgBuf);
						free(toBuf);
						free(msgBuf);
						toBuf = malloc(sizeof(char)*2);
						msgBuf = malloc(sizeof(char)*2);
					}
				}
				//newch = getch();
				break;
		}
	} else if (page == 0) {	
		if (!workTab) {		
			switch(ch) {
				case KEY_UP:
					mode = 0;
					break;
				case KEY_LEFT:
					highlight = 0;
					break;
				case KEY_RIGHT:
					highlight = 1;
					break;
				case '\n':
					if (!highlight) {
						mvprintw(10, 6,"To add a new contact:");
						mvprintw(11, 6,"place their public key in data/conts/");
						getch();
					} else {
						workTab = 1;
					}
					break;
				}
		} else {
			switch(ch) {
				case '\n': ;
					char cmd[10] = "Rob Gorrie";
					snprintf(cmd, sizeof(cmd), "rm data/conts/%s*", cmd); 
					system(cmd);
					initKeyRing();
					loadData();
					workTab = 0;
					break;
			}
		}
	}
}

// MENU CONTROL FUNCTIONS -------------------------------------------------------------------------

// MISCELLANEOUS FUNCTIONS ------------------------------------------------------------------------

void loadData() {
	contacts = malloc(sizeof(char) * (fileSize("data/contacts") + 1));
	readFile("data/contacts", contacts, fileSize("data/contacts") + 1);
	//parseContacts(contacts);
	messages = malloc(sizeof(char) * (fileSize("data/messages") + 1));
	readFile("data/messages", messages, fileSize("data/messages") + 1);
	archived = malloc(sizeof(char) * (fileSize("data/archived") + 1));
	readFile("data/archived", archived, fileSize("data/archived") + 1);
	//printf("test : %s\n", messages);
}

void parseBuffer (WINDOW *work_win, char* buffer, int start) {
	int x = 1; int y = start;
	char cur;
	int q;
	for (q=0; q < strlen(buffer); q++) {
		cur = buffer[q];
		//mvwprintw(work_win, 3, 1,"%c",contacts[q]);
		if (cur == '.') {
			x = 1;
			y++;
			q = q + 4;
		} else if (cur == '\n') {
			x = 1;
			y++;
		} else {
			mvwprintw(work_win, y, x,"%c",cur);
			x++;
		}
	}
}

// MISCELLANEOUS FUNCTIONS ------------------------------------------------------------------------
