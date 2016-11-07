#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>
#define width 4
#define height 4
#define probOf2 0.05
#define cwdt 4 //cell width
#define chorizpad 2 //cell pad vertical(in rows)
#define cvertpad 2 //cell pad vertical(in rows)
int fy, fx;   //field left top corner coordinates
int putRandVal(int f[][width], int e);
void showField(int f[][width]);
static int score = 0;
void moveHoriz(int left, int field[][width], int *empty);
void moveVert(int up, int field[][width], int *empty);
int main()
{
	initscr();
	attron(A_UNDERLINE);
	fx = (getmaxx(stdscr) - cwdt * width - (width - 1) * chorizpad) / 2;
	fy = (getmaxy(stdscr) - height - (height - 1) * cvertpad) / 2;
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	srand(time(NULL));
	int  field[height][width];
	
	int empty = width * height;
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			field[i][j] = 0;
		}
	}
	for(int i = 2 + rand()%3; i > 0; --i){
		empty = putRandVal(field, empty);
	}
	int input = 0;
	int canContinue(int field[][width]);
	while((empty != 0 || canContinue(field)) && ((input = getch()) != EOF)){
		switch(input){
			case 'w':
			case 'W':
			case KEY_UP:
				moveVert(1, field, &empty);
				break;
			case 's':
			case 'S':
			case KEY_DOWN:
				moveVert(0, field, &empty);
				break;
			case 'a':
			case 'A':
			case KEY_LEFT:
				moveHoriz(1, field, &empty);
				break;
			case 'd':
			case 'D':
			case KEY_RIGHT:
				moveHoriz(0, field, &empty);
				break;
		}
	}
	clear();
	mvprintw(fy, fx, "THE END");
	mvprintw(fy + 1, fx, "SCORE: %i", score);
	refresh();
	getchar();
	endwin();
}
int canContinue(int field[][width]){
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			if((i < height - 1) && (field[i + 1][j] == field[i][j]) || (j < width - 1) && (field[i][j + 1] == field[i][j])){
				return 1;
			}
		}
	}
	return 0;
}

void showField(int f[][width]){
	mvprintw(0, 0, "SCORE: %i", score);
	for(int i = 0; i < height; ++i){
		mvprintw(fy + i * cvertpad, fx, "||");
		for(int j = 0; j < width; ++j){
			if(f[i][j] != 0){
				printw("%4i||", f[i][j]);
			}else{
				printw("    ||");
			}
		}
	}
	refresh();
}

int nextVal(){
	return ((double)rand()/(double)RAND_MAX) < probOf2 ? 2 : 4; 
}

int putRandVal(int f[][width], int e){
	int a = rand() % (e); 
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			if(f[i][j] == 0){
				--a;
				if(a <= 0){
					f[i][j] = nextVal();
					return e - 1;
				}
			}
		}
		
	}
	return 0;
}

static int moved = 0;
static int fr = 0;
static int prev = 0;

void moveVert(int up, int field[][width], int *empty){
	moved = 0;
	for(int i = 0; i < width; ++i){
		fr = 0;
		prev = 0;
		for(int j = (up ? 0 : height - 1); (up && (j < height)) || (!up && (j >= 0)); up ? ++j : --j){
			//printf("i: %i j: %i", i, j);
			if(field[j][i] == 0){
				++fr;
				continue;
			}else if(prev == field[j][i]){
				score += field[j][i];
				field[j][i] = 0;
				field[(up ? j - fr - 1 : j + fr + 1)][i]*=2;
				++(*empty);
				++fr;
				++moved;
				prev = 0;
				continue;
			}else{
					prev = field[j][i];
				if(fr != 0){
					++moved;
					field[(up ? j - fr  : j + fr)][i] = field[j][i];
					field[j][i] = 0;
				}
			}
		}
	}
	if(moved){
		*empty = putRandVal(field, *empty);
		showField(field);
	}
}

void moveHoriz(int left, int field[][width], int *empty){
	moved = 0;
	for(int i = 0; i < height; ++i){
		fr = 0;
		prev = 0;
		for(int j = (left ? 0 : width - 1); (left && (j < width)) || (!left && (j >= 0)); left ? ++j : --j){
			//printf("i: %i  j: %i ", i, j);
			if(field[i][j] == 0){
				++fr;
				continue;
			}else if(prev == field[i][j]){
				score += field[i][j];
				field[i][j] = 0;
				field[i][(left ? j - fr - 1 : j + fr + 1)]*=2;
				++(*empty);
				++fr;
				++moved;
				prev = 0;
				continue;
			}else{
					prev = field[i][j];
				if(fr != 0){
					++moved;
					field[i][(left ? j - fr  : j + fr)] = field[i][j];
					field[i][j] = 0;
				}
			}
		}
	}
	if(moved){
		*empty = putRandVal(field, *empty);
		showField(field);
	}
}
