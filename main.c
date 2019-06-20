#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>

#define LEFT_WARD 75
#define RIGHT_WARD 77
#define UP_WARD 72
#define DOWN_WARD 80

enum direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

void gotoyx(int y, int x) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void createRandomBlock(int (*map)[4]);
void moveTo(enum direction direction, int (*map)[4]);
void printMap(int map[4][4]);
void eraseMap();
void rePrintMap(int map[4][4]);
int calcScore(int map[4][4]);
int isGameOver(int map[4][4]);

int main(void) {
    int map[4][4] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
    };
    int defaultFlag;
    srand(time(NULL));
    createRandomBlock(map);
    printMap(map);
    while(1) {
        defaultFlag=0;
        switch(getch()) {
            case LEFT_WARD:
                moveTo(LEFT, map);
                break;
            case RIGHT_WARD:
                moveTo(RIGHT, map);
                break;
            case UP_WARD:
                moveTo(UP, map);
                break;
            case DOWN_WARD:
                moveTo(DOWN, map);
                break;
            default:
                defaultFlag=1;
        }
        if(!defaultFlag) {
            if(isGameOver(map))
                break;
            createRandomBlock(map);
            rePrintMap(map);
            gotoyx(0, 25);
            printf("score: %d", calcScore(map));
        }
    }
    gotoyx(10, 0);
    puts("GameOver");
    system("pause");
    return 0;
}

void createRandomBlock(int (*map)[4]){
    int randY, randX, isAbleToMake=0;
    int i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            if(!(map[i][j]))
                isAbleToMake=1;
    if(!isAbleToMake)
        return;

    while(1){
        randY=rand()%4;
        randX=rand()%4;
        if(map[randY][randX] == 0)
            break;
    }
    if(rand()%2)
        map[randY][randX] = 2;
    else
        map[randY][randX] = 4;
}
void moveTo(enum direction direction, int (*map)[4]) {
    int i, j, r;
    switch (direction) {
        case LEFT:
            for (i = 0;i < 4; i++)
                for (j = 1; j <= 3; j++)
                    for (r = j; r > 0; r--) { // pushing block
                        if (!(map[i][r]) || map[i][r] > 10000) // check whether the block try to change is modified once or 0
                            break;
                        if (map[i][r - 1] && map[i][r - 1] != map[i][r]) // check wether crash happens
                            break;
                        if (!(map[i][r - 1])) // move block if able
                            map[i][r - 1] = map[i][r];
                        else if (map[i][r] == map[i][r - 1]) { // add if the same
                            map[i][r - 1] *= 2;
                            map[i][r - 1] += 10000;
                        }
                        map[i][r] = 0;
                    }
            break;
        case RIGHT:
            for (i = 0;i < 4; i++)
                for (j = 2; j >= 0; j--)
                    for (r = j; r < 3; r++) {
                        if (!(map[i][r]) || map[i][r] > 10000)
                            break;
                        if (map[i][r + 1] && map[i][r + 1] != map[i][r])
                            break;
                        if (!(map[i][r + 1]))
                            map[i][r + 1] = map[i][r];
                        else if (map[i][r] == map[i][r + 1]) {
                            map[i][r + 1] *= 2;
                            map[i][r + 1] += 10000;
                        }
                        map[i][r] = 0;
                    }
            break;

        case UP:
            for (j = 0;j < 4; j++)
                for (i = 1; i <= 3; i++)
                    for (r = i; r > 0; r--) {
                        if (!(map[r][j]) || map[r][j] > 10000)
                            break;
                        if (map[r - 1][j] && map[r - 1][j] != map[r][j])
                            break;
                        if (!(map[r - 1][j]) )
                            map[r - 1][j] = map[r][j];
                        else if (map[r][j] == map[r - 1][j]) {
                            map[r - 1][j] *= 2;
                            map[r - 1][j] += 10000;
                        }
                        map[r][j] = 0;
                    }
            break;

        case DOWN:
            for (j = 0;j < 4; j++)
                for (i = 2; i >= 0; i--)
                    for (r = i; r < 3; r++) {
                        if (!(map[r][j]) || map[r][j] > 10000)
                            break;
                        if (map[r + 1][j] && map[r + 1][j] != map[r][j])
                            break;
                        if (!(map[r + 1][j]))
                            map[r + 1][j] = map[r][j];
                        else if (map[r][j] == map[r + 1][j]) {
                            map[r + 1][j] *= 2;
                            map[r + 1][j] += 10000;
                        }
                        map[r][j] = 0;
                    }
            break;

    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (map[i][j] > 10000)
                map[i][j] -= 10000; //임시로 더한 10000을 감소
}

void printMap(int map[4][4]) {
    int i, j, score=0;
    for(i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
            if(map[i][j]!=0)
                printf(" %2d  ", map[i][j]);
            else
                printf("  .  ");
        puts("");
    }
}
void eraseMap() {
    int i, j, score=0;
    for(i = 0; i < 4; i++) {
        gotoyx(i, 0);
        for (j = 0; j < 4; j++)
            printf("     ");
    }
    gotoyx(0, 0);
}
void rePrintMap(int map[4][4]) {
    eraseMap();
    printMap(map);
}
int calcScore(int map[4][4]) {
    int i, j, score=0;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            score+=map[i][j];
    return score;
}
int isGameOver(int map[4][4]) {
    int i,j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            if(!map[i][j]) // check if any spaces in the map
                return 0;
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(map[i][j] == map[i+1][j] || map[i][j] == map[i][j+1]) //check if the block is able to be pushed left and up side
                return 0;
    for(i=0;i<3;i++)
        if(map[i][3] == map[i+1][3]) //check if the block is able to be pushed to down side
            return 0;
    for(j=0;j<3;j++)
        if(map[3][j] == map[3][j+1]) // check if the block is able to be pushed to right side
            return 0;
    return 1;
}