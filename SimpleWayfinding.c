#include <stdio.h>
#include <stdbool.h>
 
// 맵 상수
// #define MAP_ROW 2
// #define MAP_COLUMN 5
#define MAP_ROW 7
#define MAP_COLUMN 7
 
// 출발점 상수
#define START_ROW 0
#define START_COLUMN 0
 
// 도착점 상수
#define FINISH_ROW 6
#define FINISH_COLUMN 6
 
// 맵 상수
#define MAP_TYPE_WALL 0
#define MAP_TYPE_ROAD 1
#define MAP_TYPE_BACK 2
 
// 방향 상수
#define DIRECTION_LEFT 0
#define DIRECTION_UP 1
#define DIRECTION_RIGHT 2
#define DIRECTION_DOWN 3

// 최대 왔던길 저장 상수 
#define MAX_HISTORY 100 

// 나의 위치 구조체 만들기
typedef struct _Position
{
    int row;
    int column;
} Position;
 
// 왔던길 저장하는 배열
int stackHistory[MAX_HISTORY];
int stackCur = -1;

bool Move(int direction, int map[MAP_ROW][MAP_COLUMN], Position* position);
bool isFinish(int map[MAP_ROW][MAP_COLUMN], Position *position);
bool assertStackError();
void backWard(int stackDirection, Position *position);
void printMap(int map[MAP_ROW][MAP_COLUMN], Position *position);

int main()
{
    int i, j;
    int dir;
    int moveDist = 0, backDist = 0;
    bool isClogged = false;
 
    // 맵 만들기
    /* int map[MAP_ROW][MAP_COLUMN] =
	{
        { MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_WALL, MAP_TYPE_ROAD },
        { MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD }
    }; */
    int map[MAP_ROW][MAP_COLUMN] =
	{
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD },
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_WALL },
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_WALL },
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_ROAD },
        { MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD },
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD },
        { MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD, MAP_TYPE_WALL, MAP_TYPE_ROAD }
    };
 
    // 나의 위치 만들기, 초기 위치 선언
    Position position = { START_ROW ,START_COLUMN };
 
    printf("[나의 위치]\n");
    printf("Row : %d\nColumn : %d\n", position.row, position.column);
    
    printMap(map, &position);
 
    printf("[도착점 위치 ]\n");
    printf("Row : %d\nColumn : %d\n", FINISH_ROW, FINISH_COLUMN);
 
    printf("------------------------\n");
 
    // 도착점까지 반복
    while (!isFinish(map, &position))
	{
        // 왼쪽, 위, 오른쪽, 아래 방향 순서로 움직임
        for (dir = DIRECTION_LEFT; dir <= DIRECTION_DOWN; dir++)
		{
            printf("%d 방향으로 돕니다!\n", dir);
            
            if (Move(dir, map, &position))
			{
                printf("%d 방향으로 이동합니다!\n", dir);
                
                moveDist++;
 
                if (assertStackError())
				{
                    printf("[오류]\n");
                    printf("스택 크기가 꽉 찼습니다.\n");
                    printf("종료합니다.");
                    
                    return 1;
                }
 
                stackHistory[++stackCur] = dir;
                isClogged = false;
                
                break;
            }
            else
			{
                printf("%d 방향이 막혔습니다!\n", dir);
                
                isClogged = true;
            }
        }
 
        // 막혀 있다면, 되돌아 가자
        if (isClogged)
		{
            printf("사방이 막혔습니다! 방금 전 왔던 방향 : %d\n", stackHistory[stackCur]);
 
            // 도착점을 못 찾음
            if (backDist > 0 && stackCur == 0)
			{
                printf("[오류]\n");
                printf("도착점을 정상적인 방법으로 찾을 수 없음!\n");
                
                return 1;
            }
 
            // 왔던 길 표시
            map[position.row][position.column] = MAP_TYPE_BACK;
            backWard(stackHistory[stackCur--], &position);
            backDist++;
        }
 
        printMap(map, &position);
    }
 
    printf("도착!\n");
    printf("\n[결과]\n");
    printf("총 이동 거리 : %d\n", moveDist);
    printf("총 뒤돌아간 거리 : %d\n", backDist);
    printf("최단 거리 : %d\n", moveDist - backDist);
 
    return 0;
}
 
bool Move(int direction, int map[MAP_ROW][MAP_COLUMN], Position* position)
{
    if (map == NULL || position == NULL)
    {
        return false;
    }
 
    // 갈 수 있는 길 검사하기
    switch (direction)
	{
    case DIRECTION_LEFT:
        // 위치가 왼쪽 끝이 아닐 때
        if (position->column > 0)
		{
            // 왼쪽이 갈 수 있는 길일 때
            if (map[position->row][position->column - 1] == MAP_TYPE_ROAD)
			{
                // 왔던 길 표시하기
                map[position->row][position->column] = MAP_TYPE_BACK;
                position->column--;
                return true;
            }
        }
        
        break;
    case DIRECTION_UP:
        // 위치가 위쪽 끝이 아닐 때
        if (position->row > 0)
		{
            // 위쪽이 갈 수 있는 길일 때
            if (map[position->row - 1][position->column] == MAP_TYPE_ROAD)
			{
                // 왔던 길 표시하기
                map[position->row][position->column] = MAP_TYPE_BACK;
                position->row--;
                return true;
            }
        }
        
        break;
    case DIRECTION_RIGHT:
        // 위치가 오른쪽 끝이 아닐 때
        if (position->column < MAP_COLUMN - 1)
		{
            // 오른쪽이 갈 수 있는 길일 때
            if (map[position->row][position->column + 1] == MAP_TYPE_ROAD)
			{
                // 왔던 길 표시하기
                map[position->row][position->column] = MAP_TYPE_BACK;
                position->column++;
                return true;
            }
        }
        
        break;
    case DIRECTION_DOWN:
        // 위치가 아래쪽 끝이 아닐 때
        if (position->row < MAP_ROW - 1)
		{
            // 아래쪽이 갈 수 있는 길일 때
            if (map[position->row + 1][position->column] == MAP_TYPE_ROAD)
			{
                // 왔던 길 표시하기
                map[position->row][position->column] = MAP_TYPE_BACK;
                position->row++;
                
                return true;
            }
        }
        
        break;
    }
 
    return false;
}
 
bool isFinish(int map[MAP_ROW][MAP_COLUMN], Position *position)
{
    if (map == NULL || position == NULL)
    {
        return false;
    }
 
    // 현재 위치가 도착점인지 확인하기
    return position->row == FINISH_ROW && position->column == FINISH_COLUMN;
}
 
bool assertStackError()
{
    if (stackCur + 1 == MAX_HISTORY)
	{
		return true;	
	}
	
    return false;
}
 
void backWard(int stackDirection, Position *position)
{
    switch (stackDirection)
	{
    case DIRECTION_LEFT:
        // 오른쪽으로 이동
        position->column++;
        break;
    case DIRECTION_UP:
        // 아래쪽으로 이동
        position->row++;
        break;
    case DIRECTION_RIGHT:
        // 왼쪽으로 이동
        position->column--;
        break;
    case DIRECTION_DOWN:
        // 위쪽으로 이동
        position->row--;
        break;
    }
}

void printMap(int map[MAP_ROW][MAP_COLUMN], Position *position)
{
    int i, j;
 
    printf("------------------------\n[현재 맵]\n");
    
    for (i = 0; i < MAP_ROW; i++)
	{
        for (j = 0; j < MAP_COLUMN; j++)
		{
            if (position->row == i && position->column == j)
			{
                printf("◎");
            }
            else if (i == FINISH_ROW && j == FINISH_COLUMN)
			{
                printf("★");
            }
            else
			{
                if (map[i][j] == MAP_TYPE_ROAD)
				{
                    printf("  ");
                }
                else if (map[i][j] == MAP_TYPE_WALL)
				{
                    printf("■");
                }
                else if (map[i][j] == MAP_TYPE_BACK)
				{
                    printf("◈");
                }
            }
        }
        
        printf("\n");
    }
    
    printf("------------------------\n");
}
