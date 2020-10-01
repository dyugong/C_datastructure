#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

typedef struct {
	short int vert;
	short int horiz;
}offsets;

offsets move[4] = { {-1,0},{0,1},{1,0}, {0,-1} };

typedef struct {
	short int row;
	short int col;
	short int dir;
}element;

element stack[100];
int top;



element pop();
void push(element item);
void path(int** maze, int** mark, int EXIT_ROW, int EXIT_COL);

int main(void)
{
	int r, c;
	int EXIT_COL;
	int EXIT_ROW;
	int** maze;
	int** mark;
	FILE* f;
	char fname[] = "input2.txt";
	if (fopen_s(&f, fname, "rt") != 0)
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}
	fscanf_s(f, "%d %d\n", &EXIT_ROW, &EXIT_COL);

	maze = (int**)malloc(sizeof(int*) * EXIT_ROW);
	if (maze == NULL)
	{
		printf("메모리 할당에 문제가 있습니다.");
		exit(1);
	}
	for (r = 0; r < EXIT_COL; r++)
	{
		maze[r] = (int*)malloc(sizeof(int) * (EXIT_COL));
	}

	for (r = 0; r < EXIT_ROW; r++)
	{
		for (c = 0; c < EXIT_COL; c++)
		{
			if (c != EXIT_COL - 1)
			{
				fscanf_s(f, "%d", &maze[r][c]);
			}
			else if (c == EXIT_COL - 1)
			{
				fscanf_s(f, "%d\n", &maze[r][c]);
			}
		}
	}

	mark = (int**)malloc(sizeof(int*) * (EXIT_ROW+1));
	if (mark == NULL)
	{
		printf("메모리 할당에 문제가 있습니다.");
		exit(1);
	}
	for (r = 0; r < EXIT_COL; r++)
	{
		mark[r] = (int*)malloc(sizeof(int) * (EXIT_COL));
	}

	for (r = 0; r < EXIT_ROW; r++)
	{
		for (c = 0; c < EXIT_COL; c++)
		{
			mark[r][c] = 0;
		}
	}
	for (r = 0; r < EXIT_ROW; r++)
	{
		for (c = 0; c < EXIT_COL; c++)
		{
			printf("%d", maze[r][c]);
		}
		printf("\n");
	}
	path(maze, mark, EXIT_ROW, EXIT_COL);

	for (int i = 0; i < EXIT_ROW; i++) {
		free(maze[i]);
		free(mark[i]);
	}

	free(maze);
	free(mark);
	fclose(f);
	return 0;
}

void path(int** maze, int** mark, int EXIT_ROW, int EXIT_COL)
{
	int i, row, col, nextRow, nextCol, dir, found = FALSE;
	element position;
	mark[0][0] = 1;
	top = 0;
	stack[0].row = 0;
	stack[0].col = 0;
	stack[0].dir = 1;
	while (top > -1 && !found)
	{
		position = pop();
		row = position.row;
		col = position.col;
		dir = position.dir;
		while (dir < 4 && !found)
		{
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;
			if (nextRow == EXIT_ROW - 1 && nextCol == EXIT_COL - 1)
				found = TRUE;
			else if (nextRow<0 || nextRow>EXIT_ROW - 1 || nextCol<0 || nextCol>EXIT_COL - 1)
			{
				nextRow = row;
				nextCol = col;
				++dir;
			}
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])
			{
				mark[nextRow][nextCol] = 1;
				position.row = row;
				position.col = col;
				position.dir = ++dir;
				push(position);
				row = nextRow;
				col = nextCol;
				dir = 0;
			}
			else
				++dir;
		}
	}
	if (found)
	{
		printf("The path is :\n");
		printf("row col\n");
		for (i = 0; i <= top; i++)
		{
			printf("%2d%5d\n", stack[i].row, stack[i].col);
		}
		printf("%2d%5d\n", row, col);
		printf("%2d%5d\n", EXIT_ROW - 1, EXIT_COL - 1);
	}
	else
		printf("The maze does not have a path\n");
}

void push(element item)
{
	stack[++top] = item;
}

element pop()
{
	return stack[top--];
}