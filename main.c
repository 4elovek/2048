#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define width 4
#define height 4
#define probOf2 0.05
int putRandVal(int f[][4], int e);
void showField(int f[][4]);
static int score = 0;
void moveHoriz(int left, int field[][4], int *empty);
void moveVert(int up, int field[][4], int *empty);
int main()
{
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
	int canContinue(int field[][4]);
	while((empty != 0 || canContinue(field)) && (input = getch())){//поправить условие empty != 0
		switch(input){
			case 'w':
			case 'W':
				moveVert(1, field, &empty);
				break;
			case 's':
			case 'S':
				moveVert(0, field, &empty);
				break;
			case 'a':
			case 'A':
				moveHoriz(1, field, &empty);
				break;
			case 'd':
			case 'D':
				moveHoriz(0, field, &empty);
				break;
		}
	}
	printf("THE END\nSCORE: %i", score);
	getchar();
}
int canContinue(int field[][4]){
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
	printf("\nScore: %i\n", score);
	for(int i = 0; i < height; ++i){
		printf("\n\n\n");
		for(int j = 0; j < width; ++j){
			if(f[i][j] != 0){
				printf("%4i ", f[i][j]);
			}else{
				printf("____ ");
			}
		}
	}
	printf("\n\n-----------------\n\n\n\n\n");
}

int nextVal(){
	return ((double)rand()/(double)RAND_MAX) < probOf2 ? 2 : 4; 
}

int putRandVal(int f[][4], int e){
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

void moveVert(int up, int field[][4], int *empty){
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

void moveHoriz(int left, int field[][4], int *empty){
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