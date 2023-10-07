# include "common.h"

void init_chessman(int type, char *tableline);
void init_chessboard(int row, int col, char chessboard[][COL]) ;

int main(int argc, const char *argv[]) 
{
    setlocale(LC_ALL, "");
    initscr ();                                                                                     // 初始化ncurses
    noecho ();                                                                                      // 禁止键盘回显   
    cbreak ();                                                                                      // 禁止行缓冲
    keypad (stdscr, TRUE);                                                                          // 启动键盘映射
    curs_set (1);                                                                                   // 设置光标可见

    char chessboard[ROW][COL] = { 0 };                                                              // 初始化棋盘
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            chessboard[i][j] = BLANK;
        }
    }
    int cur_x = 0, cur_y = 0;                                                                       // 光标位置
    char current_player = BLACK;                                                                    // 当前玩家
    bool game_over = false;

    while (!game_over)
    {
        clear ();                                                                                   // 清除屏幕

	    init_chessboard(ROW, COL, chessboard);  						    // 绘制棋盘

        mvwprintw(stdscr, ROW, 0, "玩家：%lc", current_player);                                     // 显示当前玩家
        
        move (cur_x, cur_y * 2);                                                                    // 移动光标
        int ch = getch();                                                                           // 获取用户输入
        
        switch (ch)
        {
            case KEY_UP:
                if (cur_x > 0)
                    cur_x--;
                break;
            case KEY_DOWN:
                if (cur_x < ROW - 1)
                    cur_x++;
                break;
            case KEY_LEFT:
                if (cur_y > 0) 
                    cur_y--;
                break;
            case KEY_RIGHT:
                if (cur_y < COL - 1) 
                    cur_y++;
                break;
            case ' ':                                                                               // 空格键下子
                if (chessboard[cur_x][cur_y] == BLANK)                                              // 确保选中的格子是空的
                {  
                    chessboard[cur_x][cur_y] = current_player;                                      // 放下棋子
                    if (check_win (chessboard, cur_x, cur_y, current_player)) 
                    {
                        clear();                                                                    // 清除屏幕
                        init_chessboard(ROW, COL, chessboard);                                      // 重新绘制棋盘（已解决第五个子显示不出来的问题）

                        game_over = true;
                        mvwprintw(stdscr, ROW, 0, "玩家：%lc 获胜", current_player);
                        break;
                    }

                    current_player =  (current_player == BLACK) ? WHITE : BLACK;                    // 切换玩家
                }
                break;
            case 'q':
                game_over = true;
                break;
        }

    }
    if (game_over) 
    {
        mvprintw(ROW + 1, 0, "按任意键退出...");
        getch();  
    }

    endwin ();
    return 0;
}

void init_chessman(int type, char *tableline) 
{
    if (type == WHITE)
        printw("O");
    else if (type == BLACK)
        printw("x");
    else
        printw("%s", tableline);
}

void init_chessboard(int row, int col, char chessboard[][COL]) 
{
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            move(i, j*2);

            if (i == 0) {
                if (j == 0)
                    init_chessman(chessboard[i][j], "╔");
                else if (j == 14)
                    init_chessman(chessboard[i][j], "╗");
                else
                    init_chessman(chessboard[i][j], "╤");
            }
            else if (i == 14) {
                if (j == 0)
                    init_chessman(chessboard[i][j], "╚");
                else if (j == 14)
                    init_chessman(chessboard[i][j], "╝");
                else
                    init_chessman(chessboard[i][j], "╧");
            }
            else {
                if (j == 0)
                    init_chessman(chessboard[i][j], "╟");
                else if (j == 14)
                    init_chessman(chessboard[i][j], "╢");
                else
                    init_chessman(chessboard[i][j], "┼");
            }
        }
    }
}

