# include "common.h"

void init_chessman (int type, char *tableline)                                  // 初始化棋子
{
    if (type == WHITE)
        printf ("○");
    if (type == BLACK)
        printf ("●");
    if (type == BLANK)
        printf ("%s", tableline);
    
}
void init_chessboard (int row, int col, int chessboard[][COL])                  // 初始化棋盘
{
    for (int i = 0; i < row; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < col; j++)
            {
                if (j == 0)
                    init_chessman (chessboard[i][j], "╔");
                else if (j == 14)
                    init_chessman (chessboard[i][j], "╗");
                else
                    init_chessman (chessboard[i][j], "╤"); 
            } 
            printf ("\n"); 
        }
        else if (i == 14)
        {
            for (int j = 0; j < col; j++)
            {
                if (j == 0)
                    init_chessman (chessboard[i][j], "╚");
                else if (j == 14)
                    init_chessman (chessboard[i][j], "╝");
                else
                    init_chessman (chessboard[i][j], "╧"); 
            } 
            printf ("\n"); 
        }
        else
                {
            for (int j = 0; j < col; j++)
            {
                if (j == 0)
                    init_chessman (chessboard[i][j], "╟");
                else if (j == 14)
                    init_chessman (chessboard[i][j], "╢");
                else
                    init_chessman (chessboard[i][j], "┼"); 
            } 
            printf ("\n"); 
        }    
    }
}

void init_menu (void)                                                           // 初始化主菜单
{
	printf("******************************\n");
	printf("******* 欢迎使用五子棋 *******\n");
	printf("***     请选择对战方式     ***\n");
	printf("*      1.人-人对战           *\n");
	printf("*      2.复盘                *\n");
	printf("*      3.退出                *\n");
	printf("******************************\n");
	printf("请选择：");
}

bool check_full (int chessboard[][COL], int row, int col)                       // 判断棋盘是否为满
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (chessboard[i][j] == BLANK)
                return false;
        }
    }
    return true;
}

bool check_direction (int chessboard[][COL], int x, int y, int dx, int dy, int color)
{   
    for (int i = 1; i < 5; i++)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || nx >= ROW || ny < 0 || ny >= COL || chessboard[nx][ny] != color)
            return false;
    }
    return true;
}

                       
bool check_win(int chessboard[][COL], int x, int y, int color) // 判断胜负
{
    return (
        check_direction(chessboard, x, y, 1, 0, color) ||
        check_direction(chessboard, x, y, 0, 1, color) ||
        check_direction(chessboard, x, y, 1, 1, color) ||
        check_direction(chessboard, x, y, 1, -1, color)
    );
}

int get_winner(int chessboard[][COL], int x, int y) 
{
    int color = chessboard[x][y];
    if (color != BLANK && check_win(chessboard, x, y, color)) 
    {
        return color;  // 返回 BLACK 或 WHITE
    }
    return BLANK;  // 如果没有人赢，返回 BLANK
}

void person_person (void)                                                       // 人人对战
{
	int chessboard[ROW][COL] = {BLANK};
	int i, j;
	init_chessboard (ROW, COL, chessboard);

	for (int step = 1; step <= ROW * COL; step++) 
    {    
		if (step % 2 == 1) 
        {                                   
	        printf("请黑棋落子:");
			while (1) 
            {
				scanf ("%d %d", &i, &j);
				if (chessboard[i][j] != BLANK) 
                {
					printf ("该位置已有棋子，请重新输入\n");        
					continue;
				}
				if (i >= ROW || j >= COL || i < 0 || j < 0) 
                {
					printf ("输入超出棋盘范围，请重新输入\n");      
					continue;
				}
				break;
			}
			chessboard[i][j] = BLACK;

            init_chessboard (ROW, COL, chessboard);
            int winner = get_winner(chessboard, i, j);
            if (winner == BLACK) 
            {
                printf("黑棋胜 \n");
                save_chess(chessboard, ROW, COL);
                exit(0);
            }
			
		} 
        else if (step % 2 == 0) 
        {                            
			printf("请白棋落子:");
			while (1) {
				scanf("%d %d", &i, &j);
				if (chessboard[i][j] != BLANK) 
                {
					printf ("该位置已有棋子，请重新输入\n");        //棋子只能落在空白处
					continue;
				}
				if (i >= ROW || j >= COL || i < 0 || j < 0) 
                {
					printf ("输入超出棋盘范围，请重新输入\n");     //棋子坐标不可超出棋盘
					continue;
				}
				break;
			}
			chessboard[i][j] = WHITE;
            init_chessboard (ROW, COL, chessboard);
            int winner = get_winner (chessboard, i, j);
            if (winner == WHITE) 
            {
                printf("白棋胜\n");
                save_chess(chessboard, ROW, COL);
                exit(0);
            }
            
		}
	}
	if (check_full (chessboard, ROW, COL) == true)
		printf ("棋盘已满");
}

void save_chess (int chessboard[][COL], int row, int col)                       // 存盘
{
    int choice;
    FILE *fp;

    printf ("******************************\n");
	printf ("**是否结束游戏并保存当前棋局**\n");
	printf ("*      1.存盘并退出          *\n");
	printf ("*      2.继续游戏            *\n");
	printf ("******************************\n");
	printf ("请选择：");

    while (1)
    {
     
		scanf("%d", &choice);
		if (choice > 2) {
			printf("输入错误，请重新选择\n");
			continue;
		}
		break;
	}
	if (choice == 1) 
    {
		if ( ( fp = fopen( "Save_chess.txt", "w" ) ) == NULL ) 
			printf(" 保存失败\n");
		else 
        {
			for (int i = 0; i < row; i++) 
            {
				for (int j = 0; j < col; j++)
					fprintf(fp, "%d", chessboard[i][j]);
			}
			fclose (fp);
			printf ("恭喜您，保存成功");
		}
		exit(0);
    }    
}
void replay_chess (void)                                                        // 复盘
{

	int  chessboard[ROW][COL] = {BLANK};
	FILE *fp;
	if ((fp = fopen("Save_chess.txt", "w")) == NULL) 
		printf ("复盘失败");
	else 
    {
		for (int i = 0; i < ROW; i++) 
        {
			for (int j = 0; j < COL; j++) 
				fscanf(fp, "%d", &chessboard[i][j]);
		}
		fclose (fp);
		init_chessboard (ROW, COL, chessboard);
    }
}